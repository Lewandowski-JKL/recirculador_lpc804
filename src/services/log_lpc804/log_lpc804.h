#ifndef LOG_LPC804_H
#define LOG_LPC804_H

void initDebug();
void endDebug();
void SerialPrint(char string[]);
void SerialPrintln(char message[], unsigned char len);
#endif