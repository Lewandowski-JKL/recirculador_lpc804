#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <stdio.h>


typedef void (*ISR_FUNCTION)(volatile void *arg);

enum INTERRUPT_TRIGGER
{
    FALLING = 0b01,
    RISING = 0b10,
    EDGE = 0b011,
    LOW_LEVEL = 0b101,
    HIGH_LEVEL = 0b110
};

enum TIMER_MODE
{
    RepeatInterrupt = 0,
    OneShot,
    BusStall
};

void newExternInterrupt(unsigned char pin, ISR_FUNCTION isrFunc, enum INTERRUPT_TRIGGER trigger, volatile void *arg);
void newTimerInterrupt(unsigned char Timer,  ISR_FUNCTION isrFunc, enum TIMER_MODE timerMode, unsigned int ticks);
void newTimerIntFreq(unsigned char Timer,  ISR_FUNCTION isrFunc, long frequency);
void newTimerIntOneShot(unsigned char Timer,  ISR_FUNCTION isrFunc, long time_us);

void SysTickBegin(int ticks);
void SysTickBeginISR(int frequency,  ISR_FUNCTION isrFunc);
unsigned long long SysTickGetTicks();
unsigned long long SysTickGetTime_us();
unsigned long long SysTickGetTime_ms();
unsigned long long SysTickGetTime_seconds();

#endif