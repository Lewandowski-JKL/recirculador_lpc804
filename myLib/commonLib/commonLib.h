/*
 * commomLib.h
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */

#ifndef COMMONLIB_COMMONLIB_H_
#define COMMONLIB_COMMONLIB_H_

#include "${ProjDirPath}/../myLib/myADC/myADC.h"
#include "${ProjDirPath}/../myLib/myFuncControl/myFuncControl.h"
#include "${ProjDirPath}/../myLib/myGPIO/myGPIO.h"
#include "${ProjDirPath}/../myLib/myI2c/myI2c.h"
#include "${ProjDirPath}/../myLib/myInterrupt/myInterrupt.h"
#include "${ProjDirPath}/../myLib/myRegisters/myRegisters.h"
#include "${ProjDirPath}/../myLib/myFlashMemory/myFlashMemory.h"
#include "${ProjDirPath}/../myLib/checksum/checksum.h"
#include "${ProjDirPath}/../myLib/myNTC_Thermistor/myNTC_Thermistor.h"


#define MyDEBUG
//Constantes
extern const short FirmwareVersion[3];

//Extern variables
//extern SMHandle *SerialSend;
//extern SMHandle *SerialReceive;
extern unsigned char TaskRunFlag;
extern unsigned int DebugTaskFlag;
//extern int timeAuxReceive;
//Slaves Addrs
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


//#define PROTOTYPE
#ifdef PROTOTYPE
    //Pins definitions
    #define pin_nReload         12//led
    #define pin_RESET           5
    #define pin_SDA             4
    #define pin_SCL             0
    //#define pin_DEBUG_UART_TX   GPIO_NUM_21
    //#define pin_DEBUG_UART_RX   GPIO_NUM_20
    #define pin_nReady          3
    #define pin_nLink           2
    #define pin_TempMeasure     8//pin15  //1//adc
    #define pin_Botoeira        8//led
    #define pin_SensorDeFluxo   9//led
#else
    //Pins definitions
    #define pin_nReload         12
    #define pin_RESET           3
    #define pin_SDA             10//6//2
    #define pin_SCL             16//0//0
    //#define pin_DEBUG_UART_TX   GPIO_NUM_21
    //#define pin_DEBUG_UART_RX   GPIO_NUM_20
    #define pin_nReady          2
    #define pin_nLink           11
    //////////////////////////////////
    #define pin_TempMeasure     1
    #define pin_Botoeira        9
    #define pin_SensorDeFluxo   8
#endif

//Memory Sizes definitions
#define MEMORY_SIZE_1024    1024
#define MEMORY_SIZE_2048    2048
#define MEMORY_SIZE_4096    4096
#define MEMORY_SIZE_8192    8192
#define MEMORY_SIZE_16384   16384

//Time Definitions
#define TIME_1MS        1000
#define TIME_2MS        2000
#define TIME_5MS        5000
#define TIME_10MS       10000
#define TIME_20MS       20000
#define TIME_50MS       50000
#define TIME_100MS      100000
#define TIME_200MS      200000
#define TIME_500MS      500000
#define TIME_1000MS     1000000
#define TIME_2000MS     2000000
#define TIME_5000MS     5000000
#define TIME_10000MS    10000000
#define TIME_20000MS    20000000
#define TIME_50000MS    50000000

//Delays
#define DELAY_1MS       delay_us(TIME_1MS)//SDK_DelayAtLeastUs(TIME_1MS,SystemCoreClock);
#define DELAY_2MS       delay_us(TIME_2MS)//SDK_DelayAtLeastUs(TIME_2MS,SystemCoreClock);
#define DELAY_5MS       delay_us(TIME_5MS)//SDK_DelayAtLeastUs(TIME_5MS,SystemCoreClock);
#define DELAY_10MS      delay_us(TIME_10MS)//SDK_DelayAtLeastUs(TIME_10MS,SystemCoreClock);
#define DELAY_20MS      delay_us(TIME_20MS)//SDK_DelayAtLeastUs(TIME_20MS,SystemCoreClock);
#define DELAY_50MS      delay_us(TIME_50MS)//SDK_DelayAtLeastUs(TIME_50MS,SystemCoreClock);
#define DELAY_100MS     delay_us(TIME_100MS)//SDK_DelayAtLeastUs(TIME_100MS,SystemCoreClock);
#define DELAY_200MS     delay_us(TIME_200MS)//SDK_DelayAtLeastUs(TIME_200MS,SystemCoreClock);
#define DELAY_500MS     delay_us(TIME_500MS)//SDK_DelayAtLeastUs(TIME_500MS,SystemCoreClock);
#define DELAY_1000MS    delay_us(TIME_1000MS)//SDK_DelayAtLeastUs(TIME_1000MS,SystemCoreClock);
#define DELAY_2000MS    delay_us(TIME_2000MS)//SDK_DelayAtLeastUs(TIME_2000MS,SystemCoreClock);
#define DELAY_5000MS    delay_us(TIME_5000MS)//SDK_DelayAtLeastUs(TIME_5000MS,SystemCoreClock);
#define DELAY_10000MS   delay_us(TIME_10000MS)//SDK_DelayAtLeastUs(TIME_10000MS,SystemCoreClock);
#define DELAY_20000MS   delay_us(TIME_20000MS)//SDK_DelayAtLeastUs(TIME_20000MS,SystemCoreClock);
#define DELAY_50000MS   delay_us(TIME_50000MS)//SDK_DelayAtLeastUs(TIME_50000MS,SystemCoreClock);

//Debug task Flags
/*#define TASKFLAGSERIALRECEIVE   0b0000000000000001
#define TASKFLAGSERIALSEND      0b0000000000000010
#define TASKFLAGPROCESSMESSAGE  0b0000000000000100*/

enum modelMap
{
   modelSolar = 0x05,
   modelRecirculador
};

#ifdef MYREGISTERS_H
    void setBoolWithAddr(bool value, short addr);
    bool getBoolWithAddr(short addr);
    #define regMapBoolOffset 0x0448
    enum regMapBool
    {
        regMapBomba = regMapBoolOffset+1,
        regMapBotoeira,
        regMapSolenoideA,
        regMapSolenoideB,
        regMapTotalBool = regMapSolenoideB-regMapBoolOffset,
    };
    void setShortWithAddr(short value, short addr);
    short getShortWithAddr(short addr);
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
    void setIntWithAddr(int value, short addr);
    int getIntWithAddr(short addr);
    #define regMapIntOffset 0x13EB
    enum regMapInt
    {
        regMapTimestamp = regMapIntOffset+1,
        regMapAgendamentosInit,
        regMapAgendamentosEnd = regMapAgendamentosInit+9,
        regMapTotalInt = regMapAgendamentosEnd - regMapIntOffset,
    };
    void setFloatWithAddr(float value, short addr);
    float getFloatWithAddr(short addr);
    #define regMapFloatOffset 0x1BBB
    enum regMapFloat
    {
        regMapFluxo = regMapFloatOffset+1,
        regMapLitrosTotais,
        regMapTemperaturaMedida,
        regMapTemperaturaRef,
        regMapTotalFloat = (regMapTemperaturaRef-regMapFloatOffset)+1,
        regMapCalibFluxo = 0x1BC6
    };
    #define NumTotalRegisters  (regMapTotalBool + regMapTotalShort + regMapTotalInt + regMapTotalFloat)

    extern myBoolRegisters BoolRegisters[regMapTotalBool];
    extern myShortRegisters ShortRegisters[regMapTotalShort];
    extern myIntRegisters IntRegisters[regMapTotalInt];
    extern myFloatRegisters FloatRegisters[regMapTotalFloat];

    void initRegisters();
    void processMessage(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX);

#endif

/*#define clock_30M 30000
#define clock_24M 24000
#define clock_18M 18000

void setClock(int clock);*/
void setClock30M();
void setClock24M();
void setClock18M();

#ifdef MYINTERRUPT_H
void delayMs(unsigned int delay);
void delay_us(unsigned int delay);
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
