/*
 * myRegisters.h
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */

#ifndef MYREGISTERS_H
#define MYREGISTERS_H

#include <stdlib.h>
#include <stdbool.h>

enum RegCodes
{
    RegFail,
    RegOk
};

#define __LOW_MEMORY__
#ifdef __LOW_MEMORY__

typedef struct myBoolRegisters
{
    bool _value;
    unsigned short _addr;
}myBoolRegisters;
void initBoolReg(myBoolRegisters * ptr, bool value, unsigned short addr);
bool getBoolRegister(myBoolRegisters *ptrReg);
unsigned char setBoolRegister(myBoolRegisters *ptrReg, bool value);

typedef struct myShortRegisters
{
    short _value;
    short _max;
    short _min;
    unsigned short _addr;
}myShortRegisters;
void initShortReg(myShortRegisters * ptr, short value, short max, short min, unsigned short addr);
short getShortRegister(myShortRegisters *ptrReg);
unsigned char setShortRegister(myShortRegisters *ptrReg, short value);

typedef struct myIntRegisters
{
    int _value;
    int _max;
    int _min;
    unsigned short _addr;
}myIntRegisters;
void initIntReg(myIntRegisters *ptr, int value, int max, int min, unsigned short addr);
int getIntRegister(myIntRegisters *ptrReg);
unsigned char setIntRegister(myIntRegisters *ptrReg, int value);

typedef struct myFloatRegisters
{
    float _value;
    float _max;
    float _min;
    unsigned short _addr;
}myFloatRegisters;
void initFloatReg(myFloatRegisters * ptr, float value, float max, float min, unsigned short addr);
float getFloatRegister(myFloatRegisters *ptrReg);
unsigned char setFloatRegister(myFloatRegisters *ptrReg, float value);

#else

typedef struct myBoolRegisters
{
    bool _value;
    unsigned short _addr;
    bool (*get)(void *ptrReg);
    unsigned char (*set)(void *ptrReg, bool value);
}myBoolRegisters;
//myBoolRegisters *newBoolReg(bool value, unsigned short addr);
void initBoolReg(myBoolRegisters * ptr, bool value, unsigned short addr);
typedef struct myShortRegisters
{
    short _value;
    short _max;
    short _min;
    unsigned short _addr;
    short (*get)(void *ptrReg);
    unsigned char (*set)(void *ptrReg, short value);
}myShortRegisters;
myShortRegisters *newShortReg(short value, short max, short min, unsigned short addr);
void initShortReg(myShortRegisters * ptr, short value, short max, short min, unsigned short addr);
typedef struct myIntRegisters
{
    int _value;
    int _max;
    int _min;
    unsigned short _addr;
    int (*get)(void *ptrReg);
    unsigned char (*set)(void *ptrReg, int value);
}myIntRegisters;
myIntRegisters *newIntReg(int value, int max, int min, unsigned short addr);
void initIntReg(myIntRegisters *ptr, int value, int max, int min, unsigned short addr);
typedef struct myFloatRegisters
{
    float _value;
    float _max;
    float _min;
    unsigned short _addr;
    float (*get)(void *ptrReg);
    unsigned char (*set)(void *ptrReg, float value);
}myFloatRegisters;
myFloatRegisters *newFloatReg(float value, float max, float min, unsigned short addr);
void initFloatReg(myFloatRegisters * ptr, float value, float max, float min, unsigned short addr);

#endif

#endif /* MYREGISTERS_MYREGISTERS_H_ */
