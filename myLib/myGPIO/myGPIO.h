#ifndef MYGPIO_H
#define MYGPIO_H

//#include "board.h"
#include <stdbool.h>

void portBegin();

void setInputPort(unsigned char port);
void setInputPin(unsigned char pin);
bool readInputPin(unsigned char pin);

void changePin(unsigned char pin);//
void chagePort(unsigned char port);//
void writePin(unsigned char pin, unsigned char value);
void setPin(unsigned char pin);
void clearPin(unsigned char pin);
void writePort(unsigned char port, unsigned int value);
void setPort(unsigned char port);
void clearPort(unsigned char port);
void setOutputPin(unsigned char pin);
void setOutputPort(unsigned char port);



#endif