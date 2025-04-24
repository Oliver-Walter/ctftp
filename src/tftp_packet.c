#ifndef __TFTP_PACKET_C
#define __TFTP_PACKET_C

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define TFTP_MAX_PACKET_LEN 516
#define TFTP_OPERAND_BYTE 0
#define TFTP_FILE_NAME_BYTE 2
#define TFTP_BNO_BYTE 2
#define TFTP_DATA_BYTE 4
#define TFTP_ERR_CODE_BYTE 2
#define TFTP_ERR_MSG_BYTE 4
#define TFTP_MAX_DATA_LEN 512

#define MAX_FILE_NAME_LENGTH 255

#define TFTP_READ 1
#define TFTP_WRITE 2

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

/* TODO */
/* This doesnt work lol */
unsigned short getShortFromPayload(char *payload, size_t at) {
	/* if (at < TFTP_MAX_PACKET_LEN - 1) return ((payload[at] & 0xff) << 8) + (payload[at] & 0xff);
	else return -1; */
	unsigned short ret = 0;
	if (at < TFTP_MAX_PACKET_LEN - 1) {
		ret = (payload[at] & 0xff) << 8;
		ret += payload[at + 1] & 0xff;
	}
	return ret;
}

void putStringInPayload(char *payload, char *str, size_t len, size_t at) {
	size_t i;
	for (i = 0; i < len && at + i < TFTP_MAX_PACKET_LEN; i += 1) payload[at + i] = str[i];
}

size_t getStringFromPayload(char *payload, char *str_buf, size_t at) {
	size_t i, len;
	for (i = 0, len = 0; i < TFTP_MAX_PACKET_LEN; i += 1, len += 1) {
		if (payload[at + i] == 0x00) break;
		else str_buf[i] = payload[at + i];
	}
	return len;
}

_Bool isValidRequestPacket(struct tftp_packet *packet) {
	short operand = getShortFromPayload(packet->payload, 0);
	printf("%d\n", operand);
	size_t i;
	int nulls_passed = 0;
	if (!(operand == 1 || operand == 2)) return false;
	for (i = 2; i < TFTP_MAX_PACKET_LEN; i += 1) {
		switch (packet->payload[i]) {
			case 0:
				printf("test\n");
				if (nulls_passed == 1) return true;
				else nulls_passed += 1;
				break;
			default:
				break;
		}
	}
	return false;
}

void createReadRequest(struct tftp_packet *packet, char *filename, size_t filename_len, char *mode, size_t mode_len) {
	putShortInPayload(packet->payload, 1234, TFTP_OPERAND_BYTE);
	putStringInPayload(packet->payload, filename, filename_len, TFTP_FILE_NAME_BYTE);
	packet->payload[TFTP_FILE_NAME_BYTE + filename_len] = 0x0;
	putStringInPayload(packet->payload, mode, mode_len, TFTP_FILE_NAME_BYTE + filename_len + 1);
	packet->payload[TFTP_FILE_NAME_BYTE + filename_len + 1 + mode_len] = 0x0;
	packet->len = 2 + filename_len + 1 + mode_len + 1;
}

#endif /* __TFTP_PACKET_C */
