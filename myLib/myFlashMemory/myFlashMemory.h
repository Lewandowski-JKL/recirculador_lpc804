#ifndef MYFLASHMEMORY_H
#define MYFLASHMEMORY_H

#include <stdio.h>

#define IAP_FLASH_SIZE 152

enum FlashStatus
{
    FlashFail = 0,
    FlashOK
};

void iap_initFlash();
unsigned char iap_readChar(unsigned int addr);
unsigned short iap_readShort(unsigned int addr);
unsigned int iap_readInt(unsigned int addr);
unsigned char iap_writeChar(unsigned char value, unsigned int addr);
unsigned char iap_writeShort(unsigned short value, unsigned int addr);
unsigned char iap_writeInt(unsigned int value, unsigned int addr);
unsigned char iap_commit();

#endif