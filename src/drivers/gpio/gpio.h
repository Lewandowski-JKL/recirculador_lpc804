#ifndef GPIO_H
#define GPIO_H

//#include "board.h"
#include <stdbool.h>


void gpio_enableClock();
void gpio_ConfigPinInput(unsigned char pin);
void gpio_ConfigPortInput(unsigned char port);
void gpio_ConfigPinOutput(unsigned char pin);
void gpio_ConfigPortOutput(unsigned char port);
bool gpio_ReadPin(unsigned char pin);
unsigned int gpio_ReadPort(unsigned char port);
void gpio_WritePin(unsigned char pin, bool value);
void gpio_SetPin(unsigned char pin);
void gpio_ClearPin(unsigned char pin);
void gpio_TogglePin(unsigned char pin);
void gpio_WritePort(unsigned char port, unsigned int value);
void gpio_SetPort(unsigned char port, unsigned int mask);
void gpio_ClearPort(unsigned char port, unsigned int mask);
void gpio_TogglePort(unsigned char port, unsigned int mask);
void gpio_resetDisable();
void gpio_resetEnable();

#endif