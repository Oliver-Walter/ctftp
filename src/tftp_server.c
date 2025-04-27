#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "tftp_packet.c"

#define LOCAL_HOST "127.0.0.1"

#define argError(msg) {printf("Argument %s wrong", msg); exit(EXIT_FAILURE); }
#define handlePerror(msg) { perror(msg); exit(EXIT_FAILURE); }

int main(int argc, char **argv) {
	/* 
	 * Get stuff from args
	 */
	if (argc != 2) {
		printf("Usage: %s <server_port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int server_port = atoi(argv[1]);
	if (server_port == 0) argError("<server_port>");

	/*
	 * Create server socket and bind to server port
	 */
	int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd < 0) handlePerror("Socket creation");

	struct sockaddr_in client_addr, server_addr;
	bzero(&client_addr, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);

	if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) handlePerror("Socket bind");

	/*
	 * Main loop, receive a packet and start apropriate read/write job
	 */
	size_t i, recv_len;
	socklen_t addr_len;

	struct tftp_packet received_packet;
	received_packet.payload = calloc(TFTP_MAX_PACKET_LEN, sizeof(char));
	received_packet.len = 0;

	_Bool is_request;
	for (;;) {
		/* Receive packet and put into packet buffer */
		recv_len = recvfrom(socket_fd, received_packet.payload, TFTP_MAX_PACKET_LEN, 0, (struct sockaddr*)&server_addr, &addr_len);

		/* Print info about packet */
		printf("Packet received!, %lu\n", recv_len);
		printf("[ ");
		for (i = 0; i < recv_len; i += 1) printf("\"%d\", ", received_packet.payload[i]);
		printf("\b\b ]\n");

		/* Store information from packet in temporary buffers */
		/* These will be freed inside the thread they are passed to */
		unsigned short *operand = malloc(sizeof(short));
		*operand = getShortFromPayload(received_packet.payload, TFTP_OPERAND_BYTE);

		/* TODO */
		/* Create job thread based off of request packet received */
	}

	return 0;
}
