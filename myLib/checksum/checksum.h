#ifndef CHECKSUM_H
#define CHECKSUM_H
#include <stdbool.h>
unsigned short MODBUS_CRC16(unsigned char *buf, unsigned int len );
bool CHECK_CRC16(unsigned char *buf, unsigned int len );
#endif