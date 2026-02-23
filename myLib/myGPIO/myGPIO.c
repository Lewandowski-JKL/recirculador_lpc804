#include "myGPIO.h"
#include "board.h"
#include <stdbool.h>
bool gpioOn=false;
void portBegin()
{
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK);
    SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK;// reset GPIO (bit=0)
    SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK;// clear reset (bit=1)
    gpioOn = true;
}
void setInputPin(unsigned char pin)
{
    if (!gpioOn)
        portBegin();
    //GPIO->DIRSET[0] &= ~(1UL<<pin); //define o pino como entrada
    GPIO->DIRCLR[0] = (1UL<<pin); //define o pino como entrada
    gpioOn = true;    
}
void setInputPort(unsigned char port)
{
    if (!gpioOn)
        portBegin();
    GPIO->DIRCLR[port] = 0xFFFF; //define o pino como entrada
    gpioOn = true;  
}
bool readInputPin(unsigned char pin)
{
    return (bool)(GPIO->B[0][pin]);
}

void setOutputPin(unsigned char pin)
{
    if (!gpioOn)
        portBegin();
    //GPIO->DIRCLR[0] &= ~(1UL<<pin); //define o pino como saída
    GPIO->DIRSET[0] = (1UL<<pin); //define o pino como saída
    gpioOn = true;  
}
void setOutputPort(unsigned char port)
{
    if (!gpioOn)
        portBegin();
    GPIO->DIRSET[port] =  0xFFFF; //define o pino como saída
    gpioOn = true;  
}
void writePin(unsigned char pin, unsigned char value)
{
    value ? setPin(pin) : clearPin(pin);
}
void setPin(unsigned char pin)
{
    GPIO->SET[0] = (1UL<<pin);
}
void clearPin(unsigned char pin)
{
    GPIO->CLR[0] = (1UL<<pin);
}
void writePort(unsigned char port, unsigned int value)
{
    GPIO->CLR[port] = ~value;
    GPIO->SET[port] = value;
}
void setPort(unsigned char port)
{
    GPIO->SET[0] = 0xFFFF;
}
void clearPort(unsigned char port)
{
    GPIO->CLR[0] =  0xFFFF;
}

void changePin(unsigned char pin)
{
    GPIO->NOT[0] = (1UL << pin);
}
void chagePort(unsigned char port)
{
    GPIO->NOT[port] = 0xFFFF;
}