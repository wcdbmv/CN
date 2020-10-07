#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <message>\n", argv[0]);
		return EXIT_FAILURE;
	}

	const int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_ADDR_PORT),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if (sendto(sockfd, argv[1], strlen(argv[1]), 0, (struct sockaddr *) &server, sizeof server) == -1) {
		close(sockfd);
		perror("sendto");
		return EXIT_FAILURE;
	}

	tprintf("sent message: %s\n", argv[1]);

	close(sockfd);
}
