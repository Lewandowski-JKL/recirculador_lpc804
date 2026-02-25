#ifndef FLASH_MEMORY_H
#define FLASH_MEMORY_H

#include <stdio.h>

#define IAP_FLASH_SIZE 128

enum FlashStatus
{
    FlashFail = 0,
    FlashOK
};

void iap_initFlash();
unsigned char iap_readChar(unsigned int addr);
unsigned short iap_readShort(unsigned int addr);
unsigned int iap_readInt(unsigned int addr);
unsigned char iap_readVet(unsigned char *ver, unsigned short size, unsigned int addrInit);
unsigned char iap_writeChar(unsigned char value, unsigned int addr);
unsigned char iap_writeShort(unsigned short value, unsigned int addr);
unsigned char iap_writeInt(unsigned int value, unsigned int addr);
unsigned char iap_writeVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char iap_reset();
unsigned char iap_commit();

#endif