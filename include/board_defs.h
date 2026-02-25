/**
 * @file board_defs.h
 * @author Jackson Kenedi Lewandowski (jacksonleandowski.work@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-02-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef BOARD_DEFS_H
#define BOARD_DEFS_H

#define __LPC804__
#define __RECIRCULADOR__



#ifdef __RECIRCULADOR__
/*******************************************************************************
 * TIME definitions
 ******************************************************************************/
#define SysTickFrequency        (10000U)
#define SysTicksToProcess       (SysTickFrequency/1000U)   
#define SysTicks10_ms           (SysTickFrequency/100U)  
#define SysTicks100_ms          (SysTickFrequency/10U)  
#define SysTicks200_ms          (2U*SysTicks100_ms)
#define SysTicks500_ms          (5U*SysTicks100_ms)
#define SysTicks1000_ms         (SysTickFrequency/1U)  
#define SysTicks2000_ms         (2U*SysTicks1000_ms)   
#define SysTicks5000_ms         (5U*SysTicks1000_ms)
#define SysTicksButtonDebounce  (5U)
/*******************************************************************************
 * Project Definitions
 *****************************************************************************/
#define SysRecirculationTicks   (30*SysTicks1000_ms)   
#define SysRecirculationTime_ms (30000)   


/*******************************************************************************
 * Communication Definitions
 *****************************************************************************/
#define SysI2CBaudRate      (400000)//400 KHz
#define SysI2CBufferSize    (255)
#define SysI2CADDR          (0x01)

/*******************************************************************************
 * EEPROM Definitions (Flash)
 *****************************************************************************/
#define SysEepromSize       (0x70)

/*******************************************************************************
 * ADC Definitions
 *****************************************************************************/
#define SysADCTicksToRead   (SysTicks10_ms)
#define SysADCMeasureLenght (5U)
#define SysADC_TempMeasure  (10)// pin 13 adc channel 10
#define SysADC_VRef         (1)// pin 1 adc channel 1


/*******************************************************************************
 * Pin Definitions
 *****************************************************************************/
#define Syspin_nReload         (12)
#define Syspin_RESET           (5)
#define Syspin_SDA             (4)
#define Syspin_SCL             (0)
#define Syspin_UART_TX         (21)
#define Syspin_UART_RX         (20)
#define Syspin_nReady          (2)
#define Syspin_nLink           (11)
#define Syspin_TempMeasure     (13)// pin 13 adc channel 10
#define Syspin_Botoeira        (9)
#define Syspin_SensorDeFluxo   (8)
#define Syspin_Pump            (17)
#define Syspin_VRef            (1)// pin 1 adc channel 1







#endif

#endif