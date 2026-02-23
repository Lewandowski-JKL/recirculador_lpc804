/*
 * myRegister.cpp
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */
#include "myRegisters.h"

#ifdef __LOW_MEMORY__

void initBoolReg(myBoolRegisters * ptr, bool value, unsigned short addr)
{
    ptr->_addr=addr;
    ptr->_value = value;
}
void initShortReg(myShortRegisters * ptr, short value, short max, short min, unsigned short addr)
{
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->_value = value;
}
void initIntReg(myIntRegisters *ptr, int value, int max, int min, unsigned short addr)
{
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->_value = value;
}
void initFloatReg(myFloatRegisters * ptr, float value, float max, float min, unsigned short addr)
{
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->_value = value;
}

bool getBoolRegister(myBoolRegisters *ptrReg)
{
    return ptrReg->_value;
}
short getShortRegister(myShortRegisters *ptrReg)
{
    return ptrReg->_value;
}
int getIntRegister(myIntRegisters *ptrReg)
{
    return ptrReg->_value;
}
float getFloatRegister(myFloatRegisters *ptrReg)
{
    return ptrReg->_value;
}

unsigned char setBoolRegister(myBoolRegisters *ptrReg, bool value)
{
    ptrReg->_value=value;
    return RegOk;
}
unsigned char setShortRegister(myShortRegisters *ptrReg, short value)
{
    if ((value < ptrReg->_min) || (value > ptrReg->_max))
        return RegFail;
    ptrReg->_value = value;
    return RegOk;
}
unsigned char setIntRegister(myIntRegisters *ptrReg, int value)
{
    if ((value < ptrReg->_min) || (value > ptrReg->_max))
        return RegFail;
    ptrReg->_value = value;
    return RegOk;
}
unsigned char setFloatRegister(myFloatRegisters *ptrReg, float value)
{
    if ((value < ptrReg->_min) || (value > ptrReg->_max))
        return RegFail;
    ptrReg->_value = value;
    return RegOk;
}

#else

bool getBoolRegister(void *ptrReg);
short getShortRegister(void *ptrReg);
int getIntRegister(void *ptrReg);
float getFloatRegister(void *ptrReg);

unsigned char setBoolRegister(void *ptrReg, bool value);
unsigned char setShortRegister(void *ptrReg, short value);
unsigned char setIntRegister(void *ptrReg, int value);
unsigned char setFloatRegister(void *ptrReg, float value);

void initBoolReg(myBoolRegisters * ptr, bool value, unsigned short addr)
{
    ptr->_addr=addr;
    ptr->get = getBoolRegister;
    ptr->set = setBoolRegister;
    ptr->set(ptr, value);
}
void initShortReg(myShortRegisters * ptr, short value, short max, short min, unsigned short addr)
{
    ptr->get = getShortRegister;
    ptr->set = setShortRegister;
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->set(ptr, value);
}
void initIntReg(myIntRegisters *ptr, int value, int max, int min, unsigned short addr)
{
    ptr->get = getIntRegister;
    ptr->set = setIntRegister;
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->_value = value;
    //ptr->set(ptr, value);
}
void initFloatReg(myFloatRegisters * ptr, float value, float max, float min, unsigned short addr)
{
    ptr->get = getFloatRegister;
    ptr->set = setFloatRegister;
    ptr->_addr = addr;
    ptr->_max = max;
    ptr->_min = min;
    ptr->set(ptr, value);
}

myBoolRegisters *newBoolReg(bool value, unsigned short addr)
{
    myBoolRegisters *ptrAux = malloc(sizeof(myBoolRegisters));
    ptrAux->get = getBoolRegister;
    ptrAux->set = setBoolRegister;
    ptrAux->set(ptrAux, value);
    return ptrAux;
}

myShortRegisters *newShortReg(short value, short max, short min, unsigned short addr)
{
    myShortRegisters *ptrAux = (myShortRegisters*)malloc(sizeof(myShortRegisters));
    ptrAux->get = getShortRegister;
    ptrAux->set = setShortRegister;
    ptrAux->_addr = addr;
    ptrAux->_max = max;
    ptrAux->_min = min;
    ptrAux->set(ptrAux, value);
    return ptrAux;
}

myIntRegisters *newIntReg(int value, int max, int min, unsigned short addr)
{
    myIntRegisters *ptrAux = (myIntRegisters*)malloc(sizeof(myIntRegisters));
    ptrAux->get = getIntRegister;
    ptrAux->set = setIntRegister;
    ptrAux->_addr = addr;
    ptrAux->_max = max;
    ptrAux->_min = min;
    ptrAux->set(ptrAux, value);
    return ptrAux;
}

myFloatRegisters *newFloatReg(float value, float max, float min, unsigned short addr)
{
    myFloatRegisters *ptrAux = (myFloatRegisters*)malloc(sizeof(myFloatRegisters));
    ptrAux->get = getFloatRegister;
    ptrAux->set = setFloatRegister;
    ptrAux->_addr = addr;
    ptrAux->_max = max;
    ptrAux->_min = min;
    ptrAux->set(ptrAux, value);
    return ptrAux;
}


bool getBoolRegister(void *ptrReg)
{
    return ((myBoolRegisters*)ptrReg)->_value;
}
short getShortRegister(void *ptrReg)
{
    return ((myBoolRegisters*)ptrReg)->_value;
}
int getIntRegister(void *ptrReg)
{
    return ((myBoolRegisters*)ptrReg)->_value;
}
float getFloatRegister(void *ptrReg)
{
    return ((myBoolRegisters*)ptrReg)->_value;
}

unsigned char setBoolRegister(void *ptrReg, bool value)
{
    ((myBoolRegisters*)ptrReg)->_value = value;
    return RegOk;
}
unsigned char setShortRegister(void *ptrReg, short value)
{
    if ((value < ((myShortRegisters*)ptrReg)->_min) || (value > ((myShortRegisters*)ptrReg)->_max))
        return RegFail;
    ((myShortRegisters*)ptrReg)->_value = value;
    return RegOk;
}
unsigned char setIntRegister(void *ptrReg, int value)
{
    if ((value < ((myIntRegisters*)ptrReg)->_min) || (value > ((myIntRegisters*)ptrReg)->_max))
        return RegFail;
    ((myIntRegisters*)ptrReg)->_value = value;
    return RegOk;
}
unsigned char setFloatRegister(void *ptrReg, float value)
{
    if ((value < ((myFloatRegisters*)ptrReg)->_min) || (value > ((myFloatRegisters*)ptrReg)->_max))
        return RegFail;
    ((myFloatRegisters*)ptrReg)->_value = value;
    return RegOk;
}


#endif
