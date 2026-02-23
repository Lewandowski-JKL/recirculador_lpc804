
/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
//#include "pin_mux.h"
#include "clock_config.h"
//#include "peripherals.h"
//#include "board.h"
#include "fsl_debug_console.h"


//my libs
#include "mylib/commonLib/commonLib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef MyDEBUG
    #define pin_LED 17
#endif

#define SysTickFrequency    10000
#define timeTest            2
#define timeDebounce_ms     5
#define timeDebounce_us     5000
#define i2c_baud400k        400000//400000
#define i2c_BufferSize      255
#define I2C_ADDR            0x01
#define eepromSize          0x70
#define TicksToReadADC      (10)//1ms
#define LengthMeasureADC      (100)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void setup()   __attribute__ ((constructor));
void isrBotoeira(void *arg);
void isrFlow(void *arg);
void isrTimer0(void *arg);
void isrSysTick(void *arg);
void getTemp();
/*******************************************************************************
 * Global Variables
 ******************************************************************************/
NTC_Thermistor *temperature;
myADC *temperatureADC;
unsigned long timeOn_ms = 0;
unsigned long timeDebounceBotoeira = 0;
unsigned long timeDebounceFlow = 0;
unsigned char ADC_MeasureCounter = 0;
double TempMeasures = 0.0;
//myI2c *ptrMyi2c = NULL;

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Flags
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
#ifdef MyDEBUG
unsigned char message[] = {0x03, 0x03, 0xA0}; 
unsigned char messageTest[]={0x03, 0x0A, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
#endif
void setup()
{
    setClock30M();
    //setClock24M();
    //setClock18M();
    DELAY_100MS;
    portBegin();
    //Inicia pinos que se comunicam com o modulo
    setOutputPin(pin_nReload);
    clearPin(pin_nReload);//
    setOutputPin(pin_RESET);
    clearPin(pin_RESET);//
    setInputPin(pin_nReady);
    setInputPin(pin_nLink);//
    //Inicia poinos utilizados para controlar o equipamento
    setInputPin(pin_TempMeasure);
    setInputPin(pin_Botoeira);
    setInputPin(pin_SensorDeFluxo);
    temperatureADC = newADC(pin_TempMeasure,20);
    setInterrupt(pin_Botoeira,isrBotoeira,FALLING);
    setInterrupt(pin_SensorDeFluxo,isrFlow,FALLING);
    //Inicia timers
    //SysTickBegin(SystemCoreClock/SysTickFrequency);
    SysTickBeginISR(SysTickFrequency, isrSysTick);
    //inicia I2C
    i2cBeginSlave(0x01, pin_SDA, pin_SCL, i2c_baud400k);
    //inicia iap flash ("eeprom")
    iap_initFlash();
    //Inicia registradores
    initRegisters();
#ifdef MyDEBUG
    setOutputPin(pin_LED);
    clearPin(pin_LED);
    if (iap_readChar(0) != 0)
        setPin(pin_LED);
#endif
    temperature = newThermistor(110000.0,22000.0,3.3,NTC_10K_3380_VET);
}

int main(void)
{
    unsigned char bufferRX[256];
    unsigned char bufferTX[256];
    unsigned char sizeRX = 0;
    unsigned char sizeTX = 0;
    //unsigned int timeAux = 0;
    while (1)
    {
        sizeRX = readI2c_slave(bufferRX);
        if(sizeRX)
        {
            processMessage(bufferRX, &sizeRX, bufferTX, &sizeTX);
            if(sendI2c_slave(bufferTX, sizeTX) != i2cStat_OK)
                setPin(pin_nReload);
                else
                    clearPin(pin_nReload);
        }
    }
}

void isrBotoeira(void *arg)
{
    if ((SysTickGetTime_ms() - timeDebounceBotoeira) < timeDebounce_ms)
        return;
    if(readInputPin(pin_Botoeira))
        return;
    timeDebounceBotoeira = SysTickGetTime_ms();
    setBoolWithAddr(true, regMapBotoeira);
}

void isrFlow(void *arg)
{
    if ((SysTickGetTime_ms() - timeDebounceFlow) < timeDebounce_ms)
        return;
    unsigned char timeInit = 0;
    while (timeInit<timeTest)//wait 
    {
        timeInit++;
        if(readInputPin(pin_SensorDeFluxo))
            return;
    }
    timeDebounceFlow = SysTickGetTime_ms();
#ifdef MyDEBUG
changePin(pin_LED);
#endif
}

void isrTimer0(void *arg)
{

}
/***********************
 * Variaveis Sys
***********************/

void isrSysTick(void *arg)
{
    if(!(SysTickGetTicks() % TicksToReadADC))
        getTemp();
}
int vetTest[10];
void getTemp()
{
    ADC_MeasureCounter++;
    ADC_MeasureCounter%=LengthMeasureADC;
    setFloatWithAddr(readTemp_celcius(temperature, readMiliVolts(temperatureADC)), regMapTemperaturaMedida);
}
/*

#ifdef MyDEBUG
    if ((SysTickGetTime_ms()-lastTimeRead) >= 1000)
    {   
        changePin(pin_nReload);
        test++;
        if(test & 0b01)
            aux = readTest(0x4C);//58
            else
                aux = readTest(0x4C);
        lastTimeRead = SysTickGetTime_ms();
    }
    if ((SysTickGetTime_ms()-lastTimeBlink) >= delay)
    {
        if (delay == 0)
            clearPin(pin_LED);
            else if (delay == 1)
                setPin(pin_LED);
                else
                    changePin(pin_LED);
        lastTimeBlink = SysTickGetTime_ms();
    }
    //aux = readI2C(ptrMyi2c);
    switch (aux)
    {
    case i2cStat_OK:
        delay = 0;
        break;
    case i2cStat_Fail:
        delay = 50;
        break;
    case i2cStat_TimeOut:
        delay = 1;
        break;
    }
#endif
*/
