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

//COm essa definição não é utilizado calculos com numeros float
#define __NO_FLOAT__ 

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
 * Communication Definitions (I2C)
 *****************************************************************************/
#define SysI2CBaudRate          (400000)//400 KHz
#define SysI2CBufferSize        (255)
#define SysI2CADDR_WiFi         (0x01)
#define SysI2CADDR_Recirculador (0x02)

/*******************************************************************************
 * EEPROM Definitions (Flash)
 *****************************************************************************/
/*é utilizado uint8_t pq estamos nos referenciando a um endereço de memória*/
extern unsigned char __flash_end;
extern unsigned char __eeprom_start;
extern unsigned char __reg_factory;
extern unsigned char __app1_start;
extern unsigned char __app1_end;
extern unsigned char __app2_start;
extern unsigned char __app2_end;

#define SysEepromSize       (0x400)
#define SysEepromADDR       (0x00007800)

/*******************************************************************************
 * ADC Definitions
 *****************************************************************************/
#define SysADCTicksToRead   (SysTicks10_ms)
#define SysADCMeasureLenght (5U)
#define SysADC_TempS1       (3)// pin 16 adc channel 3
#define SysADC_TempS2       (10)// pin 13 adc channel 10
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
#define Syspin_TempS1          (16)// pin 16 adc channel 3
#define Syspin_TempS2          (13)// pin 13 adc channel 10
#define Syspin_Botoeira        (9)
#define Syspin_SensorDeFluxo   (8)
#define Syspin_Pump            (17)
#define Syspin_VRef            (1)// pin 1 adc channel 1


/*******************************************************************************
 * APP Definitions
 *****************************************************************************/
#define Sys_firmware_version_major    (1)
#define Sys_firmware_version_minor    (0)
#define Sys_firmware_version_patch    (0)
#define Sys_timestamp_min       (1704067200)
#define Sys_equip_code          (0x10)
#define Sys_schedulers_default  (0b11111110010110100010010110000000)

 /*******************************************************************************
 * Tasks Definitions
 *****************************************************************************/
#define Sys_task_max            (10)
#define Sys_task_name_length    (20)

 /*******************************************************************************
 * Modbus
 *****************************************************************************/
#define Sys_Modbus_Get_Bool_Reg         (0x01)
#define Sys_Modbus_Get_Num_Reg          (0x03)
#define Sys_Modbus_Set_Bool_Reg         (0x05)
#define Sys_Modbus_Set_Single_Reg       (0x06)
#define Sys_Modbus_Set_Mult_Bool_Reg    (0x0F)
#define Sys_Modbus_Set_Mult_Num_Reg     (0x10)

 /*******************************************************************************
 * Modbus Registers
 *****************************************************************************/

#define Sys_RegMap_Offset_Bool  (0x0448)
#define Sys_RegMap_Offset_Short (0x0BB8)
#define Sys_RegMap_Offset_Int   (0x13EB)
#ifndef __NO_FLOAT__
    #define Sys_RegMap_Offset_Float (0x1BBB)
    #define Sys_RegMap_End          (0x1F3F)
#else
    #define Sys_RegMap_End          (0x176F)
#endif

#define Sys_RegMap_Nreg_Bool_tot    (240)
#define Sys_RegMap_Nreg_Short_tot   (120)
#define Sys_RegMap_Nreg_Int_tot     (60)
#ifndef __NO_FLOAT__
    #define Sys_RegMap_Nreg_Float_tot   (60)
#endif


// Retorna índice relativo ou -1
#ifndef __NO_FLOAT__
    #define Sys_RegMap_GetIndex(addr) ( \
    ((addr) >= Sys_RegMap_Offset_Bool  && (addr) <  Sys_RegMap_Offset_Short) ? (int32_t)((addr) - Sys_RegMap_Offset_Bool)  : \
    ((addr) >= Sys_RegMap_Offset_Short && (addr) <  Sys_RegMap_Offset_Int)   ? (int32_t)((addr) - Sys_RegMap_Offset_Short) : \
    ((addr) >= Sys_RegMap_Offset_Int   && (addr) <  Sys_RegMap_Offset_Float) ? (int32_t)((addr) - Sys_RegMap_Offset_Int)   : \
    ((addr) >= Sys_RegMap_Offset_Float && (addr) <= Sys_RegMap_End)    ? (int32_t)((addr) - Sys_RegMap_Offset_Float) : \
    -1 )
#else
    #define Sys_RegMap_GetIndex(addr) ( \
    ((addr) >= Sys_RegMap_Offset_Bool  && (addr) <  Sys_RegMap_Offset_Short) ? (int32_t)((addr) - Sys_RegMap_Offset_Bool)  : \
    ((addr) >= Sys_RegMap_Offset_Short && (addr) <  Sys_RegMap_Offset_Int)   ? (int32_t)((addr) - Sys_RegMap_Offset_Short) : \
    ((addr) >= Sys_RegMap_Offset_Int   && (addr) <  Sys_RegMap_End) ? (int32_t)((addr) - Sys_RegMap_Offset_Int)   : \
    -1 )
#endif


#ifndef __NO_FLOAT__
    #define Sys_RegMap_Total_Bytes  ((Sys_RegMap_Nreg_Bool_tot / 8) + \
                                 (Sys_RegMap_Nreg_Short_tot * 2) + \
                                 (Sys_RegMap_Nreg_Int_tot * 4) + \
                                 (Sys_RegMap_Nreg_Float_tot * 4))
#else
    #define Sys_RegMap_Total_Bytes  ((Sys_RegMap_Nreg_Bool_tot / 8) + \
                                 (Sys_RegMap_Nreg_Short_tot * 2) + \
                                 (Sys_RegMap_Nreg_Int_tot * 4))
#endif

enum
{
    //Defines dos endereços utilizados nos registradores bool
    Sys_RegMap_Pump=(Sys_RegMap_Offset_Bool),
    Sys_RegMap_Button,
//Defines dos endereços utilizados nos registradores short
    Sys_RegMap_Model=Sys_RegMap_Offset_Short,       
    Sys_RegMap_FV_Major,     
    Sys_RegMap_FV_Minor,      
    Sys_RegMap_FV_Patch,
    Sys_RegMap_Mac_Addr_0,
    Sys_RegMap_Mac_Addr_1,
    Sys_RegMap_Mac_Addr_2,
    Sys_RegMap_ssid_0,
    Sys_RegMap_pass_0=(Sys_RegMap_ssid_0+16),
//Defines dos endereços utilizados nos registradores int
    Sys_RegMap_Timestamp= Sys_RegMap_Offset_Int,      
    Sys_RegMap_Schedules_0, Sys_RegMap_Schedules_1, Sys_RegMap_Schedules_2, Sys_RegMap_Schedules_3, Sys_RegMap_Schedules_4,
    Sys_RegMap_Schedules_5, Sys_RegMap_Schedules_6, Sys_RegMap_Schedules_7, Sys_RegMap_Schedules_8, Sys_RegMap_Schedules_9,
    Sys_RegMap_Flux,
    Sys_RegMap_Temp_S1,
    Sys_RegMap_Temp_S2,
    Sys_RegMap_Total_Liters,
    Sys_RegMap_Temp_Ref_1,
    Sys_RegMap_Temp_Ref_2,
    Sys_RegMap_Flux_Calib,
    Sys_RegMap_T1_Calib_1,
    Sys_RegMap_T1_Calib_2,
    Sys_RegMap_T1_Calib_3,
    Sys_RegMap_T2_Calib_1,
    Sys_RegMap_T2_Calib_2,
    Sys_RegMap_T2_Calib_3,
};
#define Sys_RegMap_Nreg_Bool    (2)
#define Sys_RegMap_Nreg_Short   (39)
#define Sys_RegMap_Nreg_Int     (24)//(11)
#ifndef __NO_FLOAT__
#define Sys_RegMap_Nreg_Float   (0)
#endif

// //Defines dos endereços utilizados nos registradores bool
// #define Sys_RegMap_Pump             (Sys_RegMap_Offset_Bool)
// #define Sys_RegMap_Button           (Sys_RegMap_Pump+1)
// #define Sys_RegMap_Nreg_Bool    (2)
// //Defines dos endereços utilizados nos registradores short
// #define Sys_RegMap_Model            (Sys_RegMap_Offset_Short)       
// #define Sys_RegMap_FV_Major         (Sys_RegMap_Model+1)       
// #define Sys_RegMap_FV_Minor         (Sys_RegMap_FV_Major+1)       
// #define Sys_RegMap_FV_Patch         (Sys_RegMap_FV_Minor+1) 
// #define Sys_RegMap_Mac_Addr_1       (Sys_RegMap_FV_Patch+1)
// #define Sys_RegMap_Mac_Addr_2       (Sys_RegMap_Mac_Addr_1+1)
// #define Sys_RegMap_Mac_Addr_3       (Sys_RegMap_Mac_Addr_2+1)
// #define Sys_RegMap_ssid             (Sys_RegMap_Mac_Addr_3+1)
// #define Sys_RegMap_pass             (Sys_RegMap_ssid+16)
// #define Sys_RegMap_Nreg_Short   (39)
// //Defines dos endereços utilizados nos registradores int
// #define Sys_RegMap_Timestamp        (Sys_RegMap_Offset_Int)        
// #define Sys_RegMap_Schedules        (Sys_RegMap_Timestamp+1)
// #define Sys_RegMap_Flux             (Sys_RegMap_Schedules+10)
// #define Sys_RegMap_Temp_S1          (Sys_RegMap_Flux+1)
// #define Sys_RegMap_Temp_S2          (Sys_RegMap_Temp_S1+1)
// #define Sys_RegMap_Total_Liters     (Sys_RegMap_Temp_S2+1)
// #define Sys_RegMap_Temp_Ref_1       (Sys_RegMap_Total_Liters+1)
// #define Sys_RegMap_Temp_Ref_2       (Sys_RegMap_Temp_Ref_1+1)
// #define Sys_RegMap_Flux_Calib       (Sys_RegMap_Temp_Ref_2+1)
// #define Sys_RegMap_T1_Calib_1       (Sys_RegMap_Flux_Calib+1)
// #define Sys_RegMap_T1_Calib_2       (Sys_RegMap_T1_Calib_1+1)
// #define Sys_RegMap_T1_Calib_3       (Sys_RegMap_T1_Calib_2+1)
// #define Sys_RegMap_T2_Calib_1       (Sys_RegMap_T1_Calib_3+1)
// #define Sys_RegMap_T2_Calib_2       (Sys_RegMap_T2_Calib_1+1)
// #define Sys_RegMap_T2_Calib_3       (Sys_RegMap_T2_Calib_2+1)
// #define Sys_RegMap_Nreg_Int     (24)//(11)
//Defines dos endereços utilizados nos registradores float
// #define Sys_RegMap_Flux
// #define Sys_RegMap_Temp_S1
// #define Sys_RegMap_Temp_S2
// #define Sys_RegMap_Total_Liters
// #define Sys_RegMap_Temp_Ref_1
// #define Sys_RegMap_Temp_Ref_2
// #define Sys_RegMap_Flux_Calib
// #define Sys_RegMap_Temp_Calib_1
// #define Sys_RegMap_Temp_Calib_2
// #define Sys_RegMap_Nreg_Float   (9)
// #define Sys_RegMap_Nreg_Float   (0)


#endif

#endif