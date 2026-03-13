#ifndef RECIRCULADOR_H
#define RECIRCULADOR_H

#include "system.h"
#include "board_defs.h"
#include "app_config.h"
#include "message_manager.h"
#include "registers_manager.h"

bool reg_eeprom_test(unsigned int addr);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
NTC_Thermistor temp_S1;
NTC_Thermistor temp_S2;
adc_t adc_S1;
adc_t adc_S2;
adc_t adc_Vref;
adc_t adc_Current;

/*******************************************************************************
 * Tasks
 ******************************************************************************/
void rec_get_Temp1(void *arg);
void rec_get_Temp2(void *arg);
void rec_isr_Botoeira(volatile void *arg);
void rec_isr_Flow(volatile void *arg);
void rec_system(void *arg);
void rec_error_process(void *arg);
void rec_measure(void *arg);
void rec_botoeira(void *arg);
void rec_change_verify(void *arg);

// void isrTimer0(volatile void *arg);
// void isrSysTick(volatile void *arg);

// void rec_getTemp();
// void processFunc();
// void initMyEEPROM();
// void _writeRegistersInEEPROM();
// void _writeEEPROMvaluesInRegisters();
// int myLog2(unsigned int value);
// /************************************************/
// void initQueue();
// void ControlMessageFunc(myMessage **ptrMessage, unsigned char *ptrRX, unsigned char *MessageStateControl, unsigned int *ptrTime);
// void receiveType(unsigned char *data);
// void checkBoolReg(unsigned char *data);
// void checkShortReg(unsigned char *data);
// void checkIntReg(unsigned char *data);
// void checkFloatReg(unsigned char *data);


// unsigned long timeDebounceBotoeira = 0;
// unsigned long timeDebounceFlow = 0;
// unsigned long timeAuxPumpOn = 0;
// //unsigned char flagReceiveMessage = 0;
// unsigned int FlowCounter = 0;
// myMessage messageQueue[LengthMessageQueue];
// myMessage *ptrMessage = NULL;// = messageQueue;
// myMessage *ptrPriorityMessage = NULL;
// myMessage priorityMessage;
// unsigned short wifiTypeReg = 0;

/*******************************************************************************
 * Constants
 ******************************************************************************/
// const short FirmwareVersion[3] = {0,1,0};
// const unsigned char getTypeMessage[] = {0x03, 0xB9, 0x0B, 0x27, 0x00}; 
// const unsigned char getBoolMessage[] = {0x03, 0x49, 0x04, 0x04, 0x00}; 
// //const unsigned char getShortMessage[] = {0x10, 0xB9, 0x0B, 0x04, 0x00}; 
// const unsigned char getIntMessage[] = {0x03, 0xEC, 0x13, 0x0B, 0x00}; 
// const unsigned char getFloatMessage[] = {0x03, 0xBC, 0x1B, 0x06, 0x00};
// const unsigned char setPumpMessage[] =      {0x10, 0x49, 0x04, 0x01, 0x00, 0x01, 0xFF}; 
// const unsigned char resetPumpMessage[] =    {0x10, 0x49, 0x04, 0x01, 0x00, 0x01, 0x00}; 

// //const unsigned char sendTypeMessage[] = {0x06, 0xB9, 0x0B, 0x10, 0x00}; 
/*******************************************************************************
 * Flags
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
/*******************************************************************************
 * Debug Defines
 ******************************************************************************/
// void resetReg()
// {
//     initRegisters();
//     _writeRegistersInEEPROM();
// }

#endif