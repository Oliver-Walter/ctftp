#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <pthread.h>
#include "tftp_packet.c"

#define LOCAL_HOST "127.0.0.1"

struct thread_data {
	int id;
	char *filename;
	char *mode;
};

void *readJobThread(void *thread_arg);
void *writeJobThread(void *thread_arg);

#define argError(msg) {printf("Argument error: %s\n", msg); exit(EXIT_FAILURE); }
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
	if (server_port == 0) argError("<server_port> not valid");

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

	size_t filename_len, mode_len;
	unsigned short operand;
	_Bool is_request;
	for (;;) {
		/* Receive packet and put into packet buffer */
		recv_len = recvfrom(socket_fd, received_packet.payload, TFTP_MAX_PACKET_LEN, 0, (struct sockaddr*)&server_addr, &addr_len);

		/* Print info about packet */
		printf("Packet received, length %lu\n", recv_len);
		printf("[ ");
		for (i = 0; i < recv_len; i += 1) printf("\"%d\", ", received_packet.payload[i]);
		printf("\b\b ]\n");

		/* TODO */
		/* Check if packet is formatted correctly and ignore if not */
		/* Otherwise string operations may run off the end of the payload */
		operand = getShortFromPayload(received_packet.payload, TFTP_OPERAND_BYTE);

		/* Put information into thread_data struct to be passed to thread */
		struct thread_data *thread_data = malloc(sizeof(struct thread_data));

		filename_len = strlen(&received_packet.payload[TFTP_FILE_NAME_BYTE]);
		char *filename = calloc(filename_len + 1, sizeof(char));
		filename = getStringFromPayload(received_packet.payload, TFTP_FILE_NAME_BYTE);
		printf("File name: %s\n", filename);

		mode_len = strlen(&received_packet.payload[TFTP_FILE_NAME_BYTE + filename_len + 1]);
		char* mode = calloc(mode_len + 1, sizeof(char));
		mode = getStringFromPayload(received_packet.payload, TFTP_FILE_NAME_BYTE + filename_len + 1);
		printf("Mode: %s\n\n", mode);

		/* TODO */
		/* Create job thread based off of request packet received */
		switch (operand) {
			case TFTP_RRQ:
				break;
			case TFTP_WRQ:
				break;
			default:
				printf("Invalid operand\n");
				break;
		}
	}

	return 0;
}

void *readJobThread(void *thread_arg) {
	
}
void *writeJobThread(void *thread_arg);

