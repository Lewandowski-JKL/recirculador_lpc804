#ifndef FLASH_MEMORY_H
#define FLASH_MEMORY_H

#include <stdio.h>

enum FlashStatus
{
    FlashFail = 0,
    FlashOK
};

void iap_Begin();
unsigned char iap_ReadChar(unsigned int addr);
unsigned short iap_ReadShort(unsigned int addr);
unsigned int iap_ReadInt(unsigned int addr);
unsigned char iap_ReadVet(unsigned char *ver, unsigned short size, unsigned int addrInit);
unsigned char iap_WriteChar(unsigned char value, unsigned int addr);
unsigned char iap_WriteShort(unsigned short value, unsigned int addr);
unsigned char iap_WriteInt(unsigned int value, unsigned int addr);
unsigned char iap_WriteVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char iap_Reset();
unsigned char iap_Commit();

#endif