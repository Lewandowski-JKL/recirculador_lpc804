#ifndef MYADC_H
#define MYADC_H

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_power.h"
typedef struct myADC
{
    unsigned char _channel;
    unsigned short _measureLength;
    float _convertTo_mv;
}myADC;
int readADC(myADC *adc);
float readMiliVolts(myADC *adc);
void adcBegin();
myADC *newADC(unsigned char channel, unsigned short measureLength);
//float readMiliVolts(myADC *adc);
//int readADC(myADC *adc);

#endif