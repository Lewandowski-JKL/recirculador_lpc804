/*
 * commomLib.h
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "adc.h"
#include "task.h"
#include "gpio.h"
#include "i2c.h"
#include "interrupt.h"
#include "flash_memory.h"
#include "checksum.h"
#include "ntc_thermistor.h"
#include "thermistor/NTC_10K_3380.h"
#include "calendar.h"

#define MyDEBUG
#define __LOW_MEMORY__
//Constantes
extern const short FirmwareVersion[3];
const unsigned int FlagSheduleOn = 0b10000000100000000000100000000000;

//Extern variables
//extern SMHandle *SerialSend;
//extern SMHandle *SerialReceive;
extern unsigned char TaskRunFlag;
extern unsigned int DebugTaskFlag;
//extern int timeAuxReceive;
//Slaves Addrs

//min timestamp
#define MinTimestamp 1704067200

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









#endif 
