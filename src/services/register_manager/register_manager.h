#ifndef REGISTER_MANAGER_H
#define REGISTER_MANAGER_H

#include <stdlib.h>
#include <stdbool.h>


#define WIFI_ADDR  0x01//0x01
#define MY_ADDR  0x02//0x01

//Codigo de equipamento
#define CODE_RECIRCULADOR   0x10
#define CODE_SOLAR_G2       0x11

//Functions code ModBus
#define GET_BOOLEAN_REGISTERS           0x01
#define GET_NUMERIC_REGISTERS           0x03
#define SET_BOOLEAN_REGISTER            0x05
#define SET_SINGLE_REGISTER             0x06
#define SET_MULTIPLE_BOOLEAN_REGISTERS  0x0F
#define SET_MULTIPLE_REGISTERS          0x10


const unsigned char typeBool = sizeof(bool);
const unsigned char typeChar = sizeof(char);
const unsigned char typeShort = sizeof(short);
const unsigned char typeInt = sizeof(int);
const unsigned char typeFloat = sizeof(float);


bool getBoolWithAddr(short addr);
void setBoolValue(bool value, short addr);
short getShortWithAddr(short addr);
void setShortValue(short value, short addr);
int getIntWithAddr(short addr);
void setIntValue(int value, short addr);
short getFloatWithAddr(short addr);
void setFloatValue(int value, short addr);

#define regMapBoolOffset 0x0448
enum regMapBool
{
    regMapBomba = regMapBoolOffset+1,
    regMapBotoeira,
    regMapSolenoideA,
    regMapSolenoideB,
    regMapTotalBool = regMapSolenoideB-regMapBoolOffset,
};
/* void setShortWithAddr(short value, short addr);
short getShortWithAddr(short addr);*/
#define regMapShortOffset 0x0BB8
enum regMapShort
{
    regMapModelo = regMapShortOffset+1,
    regMapFirmwareVersionInit,
    regMapFirmwareVersionEnd = regMapFirmwareVersionInit+2,
    regMapEnderecoMacInit,
    regMapEnderecoMacEnd = regMapEnderecoMacInit+2,
    regMapNomeDaRedeInit,
    regMapNomeDaRedeEnd = regMapNomeDaRedeInit+15,
    regMapSenhaDaRedeInit,
    regMapSenhaDaRedeEnd = regMapSenhaDaRedeInit+15,
    regMapTotalShort = regMapSenhaDaRedeEnd-regMapShortOffset,
};
#define regMapIntOffset 0x13EB
enum regMapInt
{
    regMapTimestamp = regMapIntOffset+1,
    regMapAgendamentosInit,
    regMapAgendamentosEnd = regMapAgendamentosInit+9,
    regMapTotalInt = regMapAgendamentosEnd - regMapIntOffset,
};
#define regMapFloatOffset 0x1BBB
enum regMapFloat
{
    regMapFluxo = regMapFloatOffset+1,
    regMapTemperaturaMedida,
    regMapLitrosTotais,
    regMapTemperaturaRef,
    regMapCalibFluxo,
    regMapCalibTemp,
    regMapTotalFloat = (regMapCalibTemp-regMapFloatOffset)
};
#define NumTotalRegisters  (regMapTotalBool + regMapTotalShort + regMapTotalInt + regMapTotalFloat)

extern volatile bool BoolRegisters[regMapTotalBool];
extern volatile short ShortRegisters[regMapTotalShort];
extern volatile int IntRegisters[regMapTotalInt];
extern volatile short FloatRegisters[regMapTotalFloat];

extern volatile bool LastBoolRegisters[regMapTotalBool];
extern volatile short LastShortRegisters[regMapTotalShort];
extern volatile int LastIntRegisters[regMapTotalInt];
extern volatile short LastFloatRegisters[regMapTotalFloat];


void initRegisters();
void processMessage(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX);

int compareBoolRegister(unsigned char *buffer);
int compareShortRegister(unsigned char *buffer);
int compareIntRegister(unsigned char *buffer);
int compareFloatRegister(unsigned char *buffer);
int compareReg(unsigned char *buffer, unsigned short addr, unsigned short numReg,
                void *regRef, void *compareReg, unsigned short size);

#ifdef MYINTERRUPT_H
void delayMs(unsigned int delay);
void delay_us(unsigned long int delay);
#endif


short copyBoolToPChar(bool *src, short size, unsigned char *dest);
short copyShortToPChar(short *src, short size, unsigned char *dest);
short copyIntToPChar(int *src, short size, unsigned char *dest);
short copyFloatToPChar(float *src, short size, unsigned char *dest);
short copyPCharToBool(unsigned char *src, short size, bool *dest);
short copyPCharToShort(unsigned char *src, short size, short *dest);
short copyPCharToInt(unsigned char *src, short size, int *dest);
short copyPCharToFloat(unsigned char *src, short size, float *dest);

#endif 
