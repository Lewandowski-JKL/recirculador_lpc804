#ifndef FLASH_MEMORY_H
#define FLASH_MEMORY_H

#include <stdio.h>
#include "board_defs.h"

enum FlashStatus
{
    FlashFail = 0,
    FlashOK
};

#ifdef __EEPROM_MEM__

void eeprom_Begin();
unsigned char eeprom_ReadChar(unsigned int addr);
unsigned short eeprom_ReadShort(unsigned int addr);
unsigned int eeprom_ReadInt(unsigned int addr);
unsigned char eeprom_ReadVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char eeprom_WriteChar(unsigned char value, unsigned int addr);
unsigned char eeprom_WriteShort(unsigned short value, unsigned int addr);
unsigned char eeprom_WriteInt(unsigned int value, unsigned int addr);
unsigned char eeprom_WriteVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char eeprom_Reset();
unsigned char eeprom_Commit();

#endif
#ifdef __IAP_MEM__

void iap_Begin();
unsigned char iap_ReadChar(unsigned int addr);
unsigned short iap_ReadShort(unsigned int addr);
unsigned int iap_ReadInt(unsigned int addr);
unsigned char iap_ReadVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char iap_WriteChar(unsigned char value, unsigned int addr);
unsigned char iap_WriteShort(unsigned short value, unsigned int addr);
unsigned char iap_WriteInt(unsigned int value, unsigned int addr);
unsigned char iap_WriteVet(unsigned char *vet, unsigned short size, unsigned int addrInit);
unsigned char iap_Reset();
unsigned char iap_Commit();

#endif


#endif