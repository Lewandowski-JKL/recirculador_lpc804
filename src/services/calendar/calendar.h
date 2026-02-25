#ifndef CALENDAR_H
#define CALENDAR_H
#include <stdbool.h>
bool schedulingTest();
unsigned char getSecond();
unsigned char getMin();
unsigned char getHour();
unsigned char getDay();
unsigned char getMount();
unsigned short getYear();
unsigned char getWeekDay();

void setTimestamp(unsigned int setTimestamp);
void tickTimestamp();
#endif