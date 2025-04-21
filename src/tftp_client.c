#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tftp_packet.c"

#define LOCAL_HOST "127.0.0.1"

#define READ 1
#define WRITE 2

#define READ_STR "read"
#define WRITE_STR "write"

#define argError(msg) {printf("Argument %s wrong", msg); exit(EXIT_FAILURE); }
#define handlePerror(msg) { perror(msg); exit(EXIT_FAILURE); }

int main(int argc, char **argv) {
	/*
	 * Get stuff from args
	 */
	if (argc != 6) {
		printf("Usage: %s <mode> <filename> <client_port> <server_address> <server_port>\n", argv[0]);
		printf("mode: read/write\n");
		exit(EXIT_FAILURE);
	}

	short mode;
	if (strcmp(argv[1], READ_STR) == 0) mode = READ;
	else if (strcmp(argv[1], WRITE_STR) == 0) mode = WRITE;
	else argError("<mode>");

	char *filename = argv[2];

	int client_port = atoi(argv[3]);
	if (client_port == 0) argError("<client_port>");

	char *server_address = argv[4];

	int server_port = atoi(argv[5]);
	if (server_port == 0) argError("<server_port>")
	
	/*
	 * Create client socket and bind to client port
	 */
	int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd < 0) handlePerror("Socket creation");

	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(client_port);

	if (bind(socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) handlePerror("Socket bind");

	/*
	 * Set client socket destination to server address
	 */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_address);

	if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) handlePerror("Socket connect");

	/*
	 * Send request packet to server
	 */
	struct tftp_packet request_packet;
	request_packet.payload = calloc(TFTP_MAX_PACKET_LEN, sizeof(char));
	putShortInPayload(&request_packet, mode, TFTP_OPERAND_BYTE);
	putStringInPayload(&request_packet, filename, strlen(filename), TFTP_FILE_NAME_BYTE);
	request_packet.len = 2 + strlen(filename);
	send(socket_fd, request_packet.payload, request_packet.len, 0);

	return 0;
}
