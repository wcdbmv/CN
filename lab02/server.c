#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "my_lltoa.h"
#include "socket.h"

static int sockfd;

void sighandler(__attribute__((unused)) int signum)
{
	close(sockfd);
	exit(EXIT_SUCCESS);
}

int perror_exit(const char *str)
{
	close(sockfd);
	perror(str);
	exit(EXIT_FAILURE);
}

void handle_client(void)
{
	struct sockaddr_in client;
	socklen_t client_address_size = sizeof client;
	char msg[BUF_SIZE];

	const ssize_t bytes = recvfrom(sockfd, msg, ARRAY_SIZE(msg), 0, (struct sockaddr *) &client, &client_address_size);
	if (bytes == -1) {
		perror_exit("recvfrom");
	}
	msg[bytes] = '\0';
	tprintf("receive message \"%s\" from %s:%d\n", msg, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	char *endptr = msg;
	const long long int decimal = strtoll(msg, &endptr, 10);
	if (!*msg || *endptr) {
		tprintf("error: \"%s\" is not a decimal number\n", msg);
		return;
	}
	tprintf("this is a decimal number: %lld\n", decimal);
	tprintf("convert to binary: %s\n", my_lltoa(decimal, msg, 2));
	tprintf("convert to hexadecimal: %s\n", my_lltoa(decimal, msg, 16));
	tprintf("convert to octal: %s\n", my_lltoa(decimal, msg, 8));
	tprintf("convert to senary: %s\n", my_lltoa(decimal, msg, 6));
}

int main(void)
{
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_ADDR_PORT),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if (bind(sockfd, (const struct sockaddr *) &server, sizeof server) == -1) {
		perror_exit("bind");
	}

	/* Find out what port was really assigned and print it */
	socklen_t namelen = sizeof server;
	if (getsockname(sockfd, (struct sockaddr *) &server, &namelen) == -1) {
		perror_exit("getsockname");
	}
	tprintf("server is running on %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

	if (signal(SIGINT, sighandler) == SIG_ERR) {
		perror_exit("signal");
	}

	for (;;) {
		handle_client();
	}
}
