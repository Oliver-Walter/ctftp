#ifndef __TFTP_PACKET_C
#define __TFTP_PACKET_C

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#define TFTP_MAX_PACKET_LEN 516
#define TFTP_OPERAND_BYTE 0
#define TFTP_FILE_NAME_BYTE 2
#define TFTP_BNO_BYTE 2
#define TFTP_DATA_BYTE 4
#define TFTP_ERR_CODE_BYTE 2
#define TFTP_ERR_MSG_BYTE 4
#define TFTP_MAX_DATA_LEN 512

#define MAX_FILE_NAME_LENGTH 255

#define TFTP_RRQ 1
#define TFTP_WRQ 2
#define TFTP_DATA 3
#define TFTP_ACK 4
#define TFTP_ERROR 5

struct tftp_packet {
	char *payload;
	size_t len;
};

void putShortInPayload(char *payload, unsigned short num, size_t at) {
	if (at < TFTP_MAX_PACKET_LEN - 1) {
		payload[at] = num >> 8;
		payload[at + 1] = num % 256;
	}
}

unsigned short getShortFromPayload(char *payload, size_t at) {
	unsigned short ret = 0;
	if (at < TFTP_MAX_PACKET_LEN - 1) {
		ret = (payload[at] & 0xff) << 8;
		ret += payload[at + 1] & 0xff;
	}
	return ret;
}

/* TODO */
/* Refactor string functions to work with null terminated strings rather that buffers and lengths */
void putStringInPayload(char *payload, char *str, size_t at) {
	size_t i;
	for (i = 0; str[i] != '\0' && at + i < TFTP_MAX_PACKET_LEN; i += 1) payload[at + i] = str[i];
}

char *getStringFromPayload(char *payload, size_t at) {
	size_t i, len = 0;
	for (i = at; payload[i] != '\0' && i < TFTP_MAX_PACKET_LEN; i += 1) len += 1;
	char *str = calloc(len + 1, sizeof(char));
	bzero(str, len + 1);
	strcpy(str, &payload[at]);
	return str;
}

void createRequestPacket(struct tftp_packet *packet, unsigned short operand, char *filename, char *mode) {
	bzero(packet->payload, TFTP_MAX_PACKET_LEN);
	putShortInPayload(packet->payload, operand, TFTP_OPERAND_BYTE);
	putStringInPayload(packet->payload, filename, TFTP_FILE_NAME_BYTE);
	putStringInPayload(packet->payload, mode, TFTP_FILE_NAME_BYTE + strlen(filename) + 1);
	packet->len = TFTP_FILE_NAME_BYTE + strlen(filename) + 1 + strlen(mode) + 1;
}

#endif /* __TFTP_PACKET_C */
