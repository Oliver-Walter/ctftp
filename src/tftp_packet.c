#ifndef __TFTP_PACKET_C
#define __TFTP_PACKET_C

#include <stdlib.h>
#include <string.h>

#define TFTP_MAX_PACKET_LEN 516
#define TFTP_OPERAND_BYTE 0
#define TFTP_FILE_NAME_BYTE 2
#define TFTP_BNO_BYTE 2
#define TFTP_DATA_BYTE 4
#define TFTP_ERR_CODE_BYTE 2
#define TFTP_ERR_MSG_BYTE 4
#define TFTP_MAX_DATA_LEN 512

struct tftp_packet {
	char *payload;
	size_t len;
};

void putShortInPayload(struct tftp_packet *packet, short n, size_t at) {
	packet->payload[at] = n >> 8;
	packet->payload[at + 1] = n % 256;
}

void putStringInPayload(struct tftp_packet *packet, char *s, size_t len, size_t at) {
	int i;
	for (i = 0; i < len && i + at < TFTP_MAX_PACKET_LEN; i += 1) packet->payload[i + at] = s[i];
}

short getShortFromPayload(struct tftp_packet *packet);
char *getStringFromPayload(struct tftp_packet *packet, size_t *len);

void putDataBytesInPayload(struct tftp_packet *packet, size_t data_len);
char *getDataBytesFromPayload(struct tftp_packet *packet, size_t data_len);

#endif /* __TFTP_PACKET_H */
