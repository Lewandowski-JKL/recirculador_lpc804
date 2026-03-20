#ifndef CHECKSUM_H
#define CHECKSUM_H
#include <stdbool.h>
unsigned short MODBUS_CRC16(unsigned char *buf, unsigned int len );
unsigned short MODBUS_STRUCT_CRC16(unsigned char *buf1, unsigned int len1, unsigned char *buf2, unsigned int len2);
bool CHECK_CRC16(unsigned char *buf, unsigned int len );
#endif