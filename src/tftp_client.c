#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tftp_packet.c"

#define LOCAL_HOST "127.0.0.1"

#define argError(msg) {printf("Argument %s wrong", msg); exit(EXIT_FAILURE); }
#define handlePerror(msg) { perror(msg); exit(EXIT_FAILURE); }

int main(int argc, char **argv) {
	/*
	 * Get stuff from args
	 */
	if (argc != 6) {
		printf("Usage: %s <action> <filename> <client_port> <server_address> <server_port>\n", argv[0]);
		printf("mode: read/write\n");
		exit(EXIT_FAILURE);
	}

	short action;
	if (strcmp(argv[1], "read") == 0) action = TFTP_READ;
	else if (strcmp(argv[1], "write") == 0) action = TFTP_WRITE;
	else argError("<action>");

	char *filename = argv[2];
	if (strlen(filename) > 255) argError("<filename>");

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
	 * Create apropriate request packet and send to server
	 */
	struct tftp_packet request_packet;
	request_packet.payload = calloc(TFTP_MAX_PACKET_LEN, sizeof(char));
	request_packet.len = 0;

	createReadRequest(&request_packet, filename, strlen(filename), "octet", strlen("octet"));
	send(socket_fd, request_packet.payload, request_packet.len, 0);

	free(request_packet.payload);
	return 0;
}
