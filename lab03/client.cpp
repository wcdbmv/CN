#include <string>
#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "const.hpp"
#include "utils.hpp"

static int client_sd;

void get_request(const std::string& uri) {
	std::ostringstream oss;
	oss << "GET /" << uri << " HTTP/1.1\n";
	oss << "Host: 127.0.0.1:" << SERVER_PORT << '\n';
	oss << "Connection: close\n";
	const auto request = oss.str();

	std::cout << "[[Request]]\n" << request << '\n';
	write(client_sd, request.c_str(), request.size());
}

void get_response() {
	char buff[MSG_LEN + 1];
	int n = 0;
	std::cout << "[[Response]]\n";
	do {
		bzero(buff, sizeof buff);
		n = read(client_sd, buff, sizeof buff - 1);
		std::cout << buff;
		fflush(stdout);
	} while (n == sizeof buff - 1);
	std::cout << std::endl;
}

int shutdown_client(const char* str) {
	close(client_sd);
	perror(str);
	return EXIT_FAILURE;
}

int main(int argc, char** argv) {
	constexpr int DEFAULT_CLIENT_PORT = 8100;
	constexpr const char* DEFAULT_URI = "index.html";

	if (argc > 3) {
		fprintf(stderr, "Usage: %s [port = %d] [uri = %s]\n", argv[0], DEFAULT_CLIENT_PORT, DEFAULT_URI);
		return EXIT_FAILURE;
	}

	int client_port = DEFAULT_CLIENT_PORT;
	std::string uri = DEFAULT_URI;

	if (argc > 1) {
		errno = 0;
		char* endptr = argv[1];
		client_port = strtol(argv[1], &endptr, 10);
		if (*endptr || errno) {
			perror("strtol");
			return EXIT_FAILURE;
		}

		constexpr int MIN_PORT = 1024;
		constexpr int MAX_PORT = 65535;
		if (client_port < MIN_PORT || client_port > MAX_PORT || client_port == SERVER_PORT) {
			fprintf(stderr, "argv[1]: invalid port\n");
			return EXIT_FAILURE;
		}

		if (argc == 3) {
			uri = argv[2];
		}
	}

	if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	const sockaddr_in client_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(client_port),
		.sin_addr = {.s_addr = INADDR_ANY},
	};

	if (reuse(client_sd) == -1) {
		return shutdown_client("setsockopt");
	}

	if (bind(client_sd, reinterpret_cast<const sockaddr*>(&client_addr), sizeof client_addr) == -1) {
		return shutdown_client("bind");
	}

	const sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_PORT),
		.sin_addr = {.s_addr = inet_addr("127.0.0.1")},
	};

	if (connect(client_sd, reinterpret_cast<const sockaddr*>(&server_addr), sizeof server_addr) == -1) {
		return shutdown_client("connect");
	}

	get_request(uri);
	get_response();

	close(client_sd);
}
