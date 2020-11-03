#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <csignal>
#include <pthread.h>
#include <queue>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <tuple>
#include "const.hpp"
#include "utils.hpp"

constexpr int THREAD_POOL_SIZE = 20;
constexpr int LISTEN_COUNT = 100;

static int server_sd;
static volatile sig_atomic_t stop;

static pthread_t thread_pool[THREAD_POOL_SIZE];

class Client {
public:
	Client(sockaddr_in client_addr, int conn_fd)
		: ip(std::string(inet_ntoa(client_addr.sin_addr)) + ":" + std::to_string(ntohs(client_addr.sin_port)))
		, socket(conn_fd)
	{ }

	const std::string ip;
	const int socket;
};

static const std::unordered_map<int, std::string> statuses{
	{200, "OK"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
};

static const std::unordered_map<std::string, std::string> content_types{
	{".html", "text/html"},
	{".css", "text/css"},
	{".js", "application/javascript"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".png", "image/png"},
	{".gif", "image/gif"},
};

std::queue<Client> queue;

pthread_mutex_t content_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
std::unordered_map<std::string, std::unordered_map<std::string, int>> content_map;

void write_statistics(const std::string& ip, const std::string& ext) {
	pthread_mutex_lock(&content_mutex);
	++content_map[ip][ext];
	pthread_mutex_unlock(&content_mutex);
}

void save_statistics() {
	auto now = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(now);

	std::string file = "statistics/";
	file += std::ctime(&end_time);
	file += ".txt";

	std::ofstream fout(file);
	for (const auto& ip : content_map) {
		fout << ip.first << " {\n";
		for (const auto& fmt : ip.second) {
			fout << "\t" << fmt.first << ": " << fmt.second << ",\n";
		}
		fout << "}\n";
	}
	fout.close();
}

void cancel_threads() {
	stop = true;
	for (auto& thread : thread_pool) {
		pthread_cancel(thread);
	}
}

void sighandler(__attribute__((unused)) int signum) {
	cancel_threads();
	close(server_sd);
	printf("[[Stop by %d]]\n", signum);
	exit(EXIT_SUCCESS);
}

std::unordered_map<std::string, std::string> get_headers(const std::string& header_text) {
	std::unordered_map<std::string, std::string> headers;
	std::istringstream header_list{ header_text };
	std::string line;
	while (std::getline(header_list, line) && line != "\n") {
		auto name_end = line.find(':');

		auto name = line.substr(0, name_end);
		auto value = line.substr(name_end + 2, line.length() - name_end - 1);

		headers[name] = value;
	}
	return headers;
}

std::string get_start_response_line(
		const std::string& protocol, int status_code, const std::string& status_string) {
	return protocol + " " + std::to_string(status_code) + " " + status_string;
}

std::string response_format(
		const std::string& protocol, int status_code, const std::string& status_string,
		std::unordered_map<std::string, std::string> headers, std::string body) {
	std::string response;
	auto start_response = get_start_response_line(
		protocol, status_code, statuses.at(status_code));
	response += start_response + "\n";

	if (status_code != 200) {
		auto response_file = "public/errors/" + std::to_string(status_code) + ".html";
		std::ifstream infile{ response_file };
		body = std::string{
			std::istreambuf_iterator<char>(infile),
			std::istreambuf_iterator<char>()
		};
		infile.close();
		headers["Content-Type"] = content_types.at(".html");
		headers["Content-Length"] = std::to_string(body.length());
	}

	std::string v[3] = {
		"Content-Type",
		"Content-Length",
		"Connection",
	};
	for (const auto& header : v) {
		if (headers.find(header) != headers.end()) {
			response += header + ": " + headers.at(header) + "\n";
		}
	}

	response += "\n" + body;
	printf("%s\n", start_response.c_str());
	return response;
}

std::tuple<std::string, std::string, std::string>
split_start_request_line(const std::string& line) {
	auto method_end = line.find(' ');
	auto path_end = line.find(' ', method_end + 1);

	auto method = line.substr(0, method_end);
	auto path = line.substr(method_end + 1, path_end - method_end - 1);
	auto protocol = line.substr(path_end + 1, line.length() - path_end - 1);
	if (path[0] == '/' && path.length() != 1) {
		path = path.substr(1, path.length() - 1);
	}
	else {
		path = "index.html";
	}
	path = "public/" + path;
	return { method, path, protocol };
}

std::string get_content_type(const std::string& ext) {
	std::string content_type;
	if (content_types.find(ext) == content_types.end()) {
		content_type = "text/plain";
	}
	else {
		content_type = content_types.at(ext);
	}
	return content_type;
}

int check_host(std::unordered_map<std::string, std::string> headers) {
	int status = 200;
	if (headers.find("Host") == headers.end()) {
		status = 404;
	}
	else {
		auto host = headers.at("Host");
		auto port = std::to_string(SERVER_PORT);
		if (host != "127.0.0.1:" + port
		    && host != "localhost:" + port
		    && host != "127.0.0.1:" + port + "\r"
		    && host != "localhost:" + port + "\r") {
			status = 404;
		}
	}
	return status;
}

std::string get_extension(const std::string& path) {
	auto dot_pos = path.find('.');
	auto ext = path.substr(dot_pos, path.length() - dot_pos);
	return ext;
}

std::string get_response(const Client& client, const std::string& request) {
	std::string response;
	int status = 200;
	std::string status_string = "OK";
	std::unordered_map<std::string, std::string> response_headers;
	response_headers["Connection"] = "close";
	std::string body;

	auto line_end = request.find('\n');
	auto line = request.substr(0, line_end);
	printf("\n[%s]  request: %s\n", client.ip.c_str(), line.c_str());
	std::cout << request << std::endl;
	printf("[%s] response: ", client.ip.c_str());

	auto headers = get_headers(request.substr(line_end + 1, request.length() - line_end));
	status = check_host(headers);
	if (status != 200) {
		response = response_format(
			"HTTP/1.1", status, statuses.at(status),
			response_headers, body
		);
		return response;
	}

	auto [method, path, protocol] = split_start_request_line(line);
	if (method != "GET" || protocol != "HTTP/1.1") {
		status = 405;
		response = response_format(
			protocol, status, statuses.at(status),
			response_headers, body
		);
		return response;
	}

	std::ifstream infile{ path };
	if (!infile.good()) {
		status = 404;
		response = response_format(
			protocol, status, statuses.at(status),
			response_headers, body
		);
		return response;
	}
	body = std::string{
		std::istreambuf_iterator<char>(infile),
		std::istreambuf_iterator<char>()
	};
	infile.close();

	auto ext = get_extension(path);
	write_statistics(client.ip, ext);

	response_headers["Content-Type"] = get_content_type(ext);
	int idx = response_headers["Content-Type"].find("image");
	if (idx != std::string::npos) {
		body = string_to_hex(body);
	}
	response_headers["Content-Length"] = std::to_string(body.length());

	response = response_format(
		protocol, status, statuses.at(status),
		response_headers, body
	);

	return response;
}

void handle_function(const Client& client) {
	char buff[MSG_LEN + 1];
	bzero(buff, sizeof buff);
	read(client.socket, buff, sizeof buff);

	std::string request = buff;
	auto response = get_response(client, request);

	write(client.socket, response.c_str(), response.size());
}

void* thread_function(void* argv) {
	while (!stop) {
		Client* pclient = nullptr;

		pthread_mutex_lock(&mutex);
		if (queue.empty()) {
			pthread_cond_wait(&condvar, &mutex);
			pclient = &queue.front();
			queue.pop();
		}
		pthread_mutex_unlock(&mutex);

		if (pclient) {
			handle_function(*pclient);
		}
	}
	return nullptr;
}

int shutdown(const char* str) {
	cancel_threads();
	close(server_sd);
	perror(str);
	return EXIT_FAILURE;
}

int main() {
	for (auto& thread : thread_pool) {
		pthread_create(&thread, nullptr, thread_function, nullptr);
	}

	if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cancel_threads();
		perror("socket");
		return EXIT_FAILURE;
	}

	const sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_PORT),
		.sin_addr = {.s_addr = INADDR_ANY},
	};

	if (reuse(server_sd) == -1) {
		return shutdown("setsockopt");
	}

	if (bind(server_sd, reinterpret_cast<const sockaddr*>(&server_addr), sizeof server_addr) == -1) {
		return shutdown("bind");
	}

	if (listen(server_sd, LISTEN_COUNT) == -1) {
		return shutdown("listen");
	}

	if (signal(SIGINT, sighandler) == SIG_ERR) {
		return shutdown("signal");
	}

	printf("server is running on %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
	for (;;) {
		sockaddr_in client_addr{};
		socklen_t client_size = sizeof client_addr;
		const int conn_fd = accept(server_sd, reinterpret_cast<sockaddr*>(&client_addr), &client_size);
		if (conn_fd == -1) {
			return shutdown("accept");
		}

		Client new_client(client_addr, conn_fd);

		pthread_mutex_lock(&mutex);
		queue.push(new_client);
		pthread_cond_signal(&condvar);
		pthread_mutex_unlock(&mutex);
	}
}
