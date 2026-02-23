#ifndef MYFLASHMEMORY_H
#define MYFLASHMEMORY_H

#include <stdio.h>

enum FlashStatus
{
    FlashFail = 0,
    FlashOK
};

typedef struct myFlash
{
    unsigned char _data[256];
    unsigned int _size;
    unsigned int _initAddr;
    unsigned int _FlashSector;
    unsigned int _FlashPage;
}myFlash;

myFlash *newFlash(unsigned int size);


unsigned char iap_readChar(myFlash *ptrFlash, unsigned int addr);
unsigned short iap_readShort(myFlash *ptrFlash, unsigned int addr);
unsigned int iap_readInt(myFlash *ptrFlash, unsigned int addr);
unsigned char iap_writeChar(myFlash *ptrFlash, unsigned char value, unsigned int addr);
unsigned char iap_writeShort(myFlash *ptrFlash, unsigned short value, unsigned int addr);
unsigned char iap_writeInt(myFlash *ptrFlash, unsigned int value, unsigned int addr);
unsigned char iap_commit(myFlash *ptrFlash);


unsigned char writeCharTest(unsigned char value, unsigned int addr);
unsigned char readCharFlashTest(unsigned int addr);
#endif