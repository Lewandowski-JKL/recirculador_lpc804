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
//#define __IAP_MEM__
#define __EEPROM_MEM__

//COm essa definição não é utilizado calculos com numeros float
#define __NO_FLOAT__ 
#define __LOW_MEMORY__

//#ifdef __RECIRCULADOR__
/*******************************************************************************
 * TIME definitions
 ******************************************************************************/
#define SysTickFrequency        (2000U)
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
#define SysI2C_ADDR_ALL          (0x00)
//Endereço de Placas Mestres                    (0x10 <-> 0x1F)
#define SysI2C_ADDR_Recirculador (0x10)
//Endereço de extensores IO e adaptadores LCD   (0x20 <-> 0x27)
//Disponível                                    (0x28 <-> 0x2F)
//Endereço de equipamentos de conectividade     (0x30 <-> 0x3D)
#define SysI2C_ADDR_WiFi         (0x30)
//Endereço de extensores IO e adaptadores LCD   (0x3E <-> 0x3F)
//Endereço de equipamentos de interface         (0x40 <-> 0x4F)
#define SysI2C_ADDR_Rec_Display  (0x40)
//Endereço de equipamentos de interface         (0x50 <-> 0x57)
//Endereço de periféricos                       (0x60 <-> 0x6F)
#define SysI2C_ADDR_RTC_BQ32000  (0x68)
//Endereço de Multiplexadore I2C                (0x70 <-> 0x77)

char SysI2C_ADDR_Slave_Vet[] = {SysI2C_ADDR_WiFi, SysI2C_ADDR_Rec_Display};

/*******************************************************************************
 * EEPROM Definitions (Flash)
 *****************************************************************************/
/*é utilizado uint8_t pq estamos nos referenciando a um endereço de memória*/
extern unsigned char __flash_end;
extern unsigned char __eeprom_start;
extern unsigned char __reg_factory;
extern unsigned char __app0_start;
extern unsigned char __app0_end;
extern unsigned char __app1_start;
extern unsigned char __app1_end;

#define SysEepromSize       (0x380)
#define SysEepromADDR       (0x7800)

/*******************************************************************************
 * ADC Definitions
 *****************************************************************************/
#define SysADCTicksToRead   (SysTicks10_ms)
#define SysADCMeasureLenght (5U)
#define SysADC_TempS1       (3)// pin 16 adc channel 3
#define SysADC_TempS2       (10)// pin 13 adc channel 10
#define SysADC_VRef         (0)// pin 1 adc channel 0
#define SysADC_Current      (8)// pin 15 adc channel 8

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
#define Syspin_VRef            (1)// pin 1 adc channel 0
#define Syspin_Current         (15)// pin 15 adc channel 8

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
#define Sys_task_max            (12)
#define Sys_task_name_length    (20)

 /*******************************************************************************
 * Modbus
 *****************************************************************************/
#define Sys_Modbus_Get_Bool_Reg         (0x01)
#define Sys_Modbus_Get_Discrete_Input   (0x02)
#define Sys_Modbus_Get_Num_Reg          (0x03)
#define Sys_Modbus_Read_Input_Reg       (0x04)
#define Sys_Modbus_Set_Bool_Reg         (0x05)
#define Sys_Modbus_Set_Single_Reg       (0x06)
#define Sys_Modbus_Read_Exception       (0x07)
#define Sys_Modbus_Diagnostics          (0x08)
#define Sys_Modbus_Get_Event_Counter    (0x0B)
#define Sys_Modbus_Get_Event_Log        (0x0C)
#define Sys_Modbus_Set_Mult_Bool_Reg    (0x0F)
#define Sys_Modbus_Set_Mult_Num_Reg     (0x10)
#define Sys_Modbus_Report_Slave_ID      (0x11)
#define Sys_Modbus_Read_File_Record     (0x14)
#define Sys_Modbus_Write_File_Record    (0x15)
#define Sys_Modbus_Mask_Write_Register  (0x16)
#define Sys_Modbus_RW_Mult_Registers    (0x17)
#define Sys_Modbus_Read_FIFO_Queue      (0x18)
#define Sys_Modbus_Read_Device_ID       (0x2B)
#define Sys_Modbus_Error                (0x80)
#define Sys_Modbus_Message_Queue_Size   (15)    //Quantidade de mensagens que podem esparar pelo envio
#define Sys_Modbus_Time_Reply           (5)    //Tempo de espera por uma resposta
#define Sys_Modbus_Send_Fail            (5)     //Quantidade de vezes que tenta enviar uma mesma mensagem

 /*******************************************************************************
 * Modbus Registers
 *****************************************************************************/

#define Sys_RegMap_Offset_Bool  (0x03E8)//(0x0448)
#define Sys_RegMap_Offset_Short (0x0BB8)//(0x0BB8)
#define Sys_RegMap_Offset_Int   (0x1388)//(0x13EB)

#ifndef __NO_FLOAT__
    #define Sys_RegMap_Offset_Float (0x1BBB)
    #define Sys_RegMap_End          (0x1F3F)
#else
    #define Sys_RegMap_End          (0x176F)
#endif

#define Sys_RegMap_Nreg_Bool_tot    (128)
#define Sys_RegMap_Nreg_Short_tot   (128)
#define Sys_RegMap_Nreg_Int_tot     (128)

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
    (((addr) >= Sys_RegMap_Offset_Bool ) && ((addr) <  Sys_RegMap_Offset_Short)) ? (int32_t)((addr) - Sys_RegMap_Offset_Bool)  : \
    (((addr) >= Sys_RegMap_Offset_Short) && ((addr) <  Sys_RegMap_Offset_Int)  ) ? (int32_t)((addr) - Sys_RegMap_Offset_Short) : \
    (((addr) >= Sys_RegMap_Offset_Int  ) && ((addr) <  Sys_RegMap_End)         ) ? (int32_t)((addr) - Sys_RegMap_Offset_Int)   : \
    -1 )
#endif

// Retorna o tamanho do registrador em bytes
#ifndef __NO_FLOAT__
    #define Sys_RegMap_GetRegBytes(addr) ( \
    ((addr) >= Sys_RegMap_Offset_Bool  && (addr) <  Sys_RegMap_Offset_Short) ? 0             : \
    ((addr) >= Sys_RegMap_Offset_Short && (addr) <  Sys_RegMap_Offset_Int)   ? sizeof(short) : \
    ((addr) >= Sys_RegMap_Offset_Int   && (addr) <  Sys_RegMap_Offset_Float) ? sizeof(int)   : \
    ((addr) >= Sys_RegMap_Offset_Float && (addr) <= Sys_RegMap_End)    ? sizeof(float)       : \
    -1 )
#else
    #define Sys_RegMap_GetRegBytes(addr) ( \
    (((addr) >= Sys_RegMap_Offset_Bool ) && ((addr) <  Sys_RegMap_Offset_Short)) ? 0             : \
    (((addr) >= Sys_RegMap_Offset_Short) && ((addr) <  Sys_RegMap_Offset_Int)  ) ? sizeof(short) : \
    (((addr) >= Sys_RegMap_Offset_Int  ) && ((addr) <  Sys_RegMap_End)         ) ? sizeof(int)   : \
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
    Sys_RegMap_Reset_Button,
    ///////////////////////////////////////////////////////////////////////
//Defines dos endereços utilizados nos registradores short
    Sys_RegMap_Model=Sys_RegMap_Offset_Short,       
    Sys_RegMap_FV_Major,     
    Sys_RegMap_FV_Minor,      
    Sys_RegMap_FV_Patch,
    Sys_RegMap_Mac_Addr_0,
    Sys_RegMap_Mac_Addr_1,
    Sys_RegMap_Mac_Addr_2,
    Sys_RegMap_ssid_0,
    Sys_RegMap_pass_0 = (Sys_RegMap_ssid_0+16),
    Sys_RegMap_eeprom_reg_bool_0    = (Sys_RegMap_pass_0+16),
    Sys_RegMap_eeprom_reg_short_0   = (Sys_RegMap_eeprom_reg_bool_0+8),
    Sys_RegMap_eeprom_reg_int_0     = (Sys_RegMap_eeprom_reg_short_0+8),
    Sys_RegMap_eeprom_reg_float_0   = (Sys_RegMap_eeprom_reg_int_0+8),
    Sys_RegMap_priority_reg_bool_0  = (Sys_RegMap_eeprom_reg_float_0+8),
    Sys_RegMap_priority_reg_short_0 = (Sys_RegMap_priority_reg_bool_0+8),
    Sys_RegMap_priority_reg_int_0   = (Sys_RegMap_priority_reg_short_0+8),
    Sys_RegMap_priority_reg_float_0 = (Sys_RegMap_priority_reg_int_0+8),
    ////////////////////////////////////////////////////////////////////////////////////
//Defines dos endereços utilizados nos registradores int
//Primeiro vem os registradores que vao se alterar, como medidas e afins
	//Timestamp
    Sys_RegMap_Timestamp = Sys_RegMap_Offset_Int, 
	//Leituras de fluxo
    Sys_RegMap_Flux_Counter,
    Sys_RegMap_Flux_Liters,
    Sys_RegMap_Flux_Total_Liters,
	//Leituras S1
    Sys_RegMap_S1_Adc,
    Sys_RegMap_S1_mV,
    Sys_RegMap_S1_Temp,
	//Leituras S2
    Sys_RegMap_S2_Adc,
    Sys_RegMap_S2_mV,
    Sys_RegMap_S2_Temp,
	//Leituras Corrente
    Sys_RegMap_Current_Adc,
    Sys_RegMap_Current_mV,
    Sys_RegMap_Current,
    //Alarmes e erros
    Sys_RegMap_Errors,
	//Valores estaticos
    //Registradores paramétricas da leitura de fluxo
    Sys_RegMap_Flux_Calib,
    Sys_RegMap_Flux_Error_Max,
    Sys_RegMap_Flux_Error_Min,
    //Registradores paramétricas do S1
    Sys_RegMap_S1_Temp_Ref,
    Sys_RegMap_S1_Temp_Hysteresis,
    Sys_RegMap_S1_Calib_1,
    Sys_RegMap_S1_Calib_2,
    Sys_RegMap_S1_Calib_3,
    Sys_RegMap_S1_Error_High,
    Sys_RegMap_S1_Error_Low,
    Sys_RegMap_S1_Error_Desconnect,
    Sys_RegMap_S1_Error_Short_Circuit,
    //Registradores paramétricas do S2
    Sys_RegMap_S2_Temp_Ref,
    Sys_RegMap_S2_Temp_Hysteresis,
    Sys_RegMap_S2_Calib_1,
    Sys_RegMap_S2_Calib_2,
    Sys_RegMap_S2_Calib_3,
    Sys_RegMap_S2_Error_High,
    Sys_RegMap_S2_Error_Low,
    Sys_RegMap_S2_Error_Desconnect,
    Sys_RegMap_S2_Error_Short_Circuit,
    //Registradores paramétricas do Corrente
    Sys_RegMap_Current_Resistor,
    Sys_RegMap_Current_Calib,
    Sys_RegMap_Current_Error_Desconnect,
    Sys_RegMap_Current_Error_Short_Circuit,
    //Tempos
    Sys_RegMap_Time_Recirculation,
    Sys_RegMap_Temp_Ref_Recirculation,
	//Agendamentos
    Sys_RegMap_Schedules_0, Sys_RegMap_Schedules_1, Sys_RegMap_Schedules_2, Sys_RegMap_Schedules_3, Sys_RegMap_Schedules_4,
    Sys_RegMap_Schedules_5, Sys_RegMap_Schedules_6, Sys_RegMap_Schedules_7, Sys_RegMap_Schedules_8, Sys_RegMap_Schedules_9
};
/*    Sys_RegMap_Timestamp = Sys_RegMap_Offset_Int,      
    Sys_RegMap_Schedules_0, Sys_RegMap_Schedules_1, Sys_RegMap_Schedules_2, Sys_RegMap_Schedules_3, Sys_RegMap_Schedules_4,
    Sys_RegMap_Schedules_5, Sys_RegMap_Schedules_6, Sys_RegMap_Schedules_7, Sys_RegMap_Schedules_8, Sys_RegMap_Schedules_9,
    //Registradores de leitura de fluxo
    Sys_RegMap_Flux_Counter,
    Sys_RegMap_Flux_Liters,
    Sys_RegMap_Flux_Total_Liters,
    Sys_RegMap_Flux_Calib,
    Sys_RegMap_Flux_Error_Max,
    Sys_RegMap_Flux_Error_Min,
    //Registradores de leitura de temperatura s1
    Sys_RegMap_S1_Adc,
    Sys_RegMap_S1_mV,
    Sys_RegMap_S1_Temp,
    Sys_RegMap_S1_Temp_Ref,
    Sys_RegMap_S1_Temp_Hysteresis,
    Sys_RegMap_S1_Calib_1,
    Sys_RegMap_S1_Calib_2,
    Sys_RegMap_S1_Calib_3,
    Sys_RegMap_S1_Error_High,
    Sys_RegMap_S1_Error_Low,
    Sys_RegMap_S1_Error_Desconnect,
    Sys_RegMap_S1_Error_Short_Circuit,
    //Registradores de leitura de temperatura s2
    Sys_RegMap_S2_Adc,
    Sys_RegMap_S2_mV,
    Sys_RegMap_S2_Temp,
    Sys_RegMap_S2_Temp_Ref,
    Sys_RegMap_S2_Temp_Hysteresis,
    Sys_RegMap_S2_Calib_1,
    Sys_RegMap_S2_Calib_2,
    Sys_RegMap_S2_Calib_3,
    Sys_RegMap_S2_Error_High,
    Sys_RegMap_S2_Error_Low,
    Sys_RegMap_S2_Error_Desconnect,
    Sys_RegMap_S2_Error_Short_Circuit,
    //Registradores de leitura de corrente
    Sys_RegMap_Current_Adc,
    Sys_RegMap_Current_mV,
    Sys_RegMap_Current,
    Sys_RegMap_Current_Resistor,
    Sys_RegMap_Current_Calib,
    Sys_RegMap_Current_Error_Desconnect,
    Sys_RegMap_Current_Error_Short_Circuit,
    //Configurações
    Sys_RegMap_Time_Recirculation,
    Sys_RegMap_Temp_Ref_Recirculation,
    //Alarmes e erros
    Sys_RegMap_Errors
};*/
#define Sys_RegMap_Nreg_Bool    (Sys_RegMap_Reset_Button + 1             -Sys_RegMap_Offset_Bool)    //2 ->   1Byte
#define Sys_RegMap_Nreg_Short   ((Sys_RegMap_priority_reg_float_0+8) + 1 -Sys_RegMap_Offset_Short)   //104 -> 208 Bytes
#define Sys_RegMap_Nreg_Int     (Sys_RegMap_Errors + 1                   -Sys_RegMap_Offset_Int)     //51 -> 204 Bytes (11)

#ifndef __NO_FLOAT__
    #define Sys_RegMap_Nreg_Float   (0)
#else
    #define Sys_RegMap_Nreg_Total   (Sys_RegMap_Nreg_Bool + Sys_RegMap_Nreg_Short + Sys_RegMap_Nreg_Int)
    #define Sys_RegMap_Nreg_Total_Bytes     ((Sys_RegMap_Nreg_Bool/8+1)                     \
                                            + (Sys_RegMap_Nreg_Short    *   sizeof(short))  \
                                            + (Sys_RegMap_Nreg_Int      *   sizeof(int)))
#endif

 /*******************************************************************************
 * Errors
 *****************************************************************************/
#define Sys_ERROR_NO                    0b00000000000000000000000000000000
/////////////////////////////////////////////////////
#define Sys_ERROR_S1_HIGH               0b00000001
#define Sys_ERROR_S1_LOW                0b00000010
#define Sys_ERROR_S1_SHORT_CIRCUIT      0b00000100
#define Sys_ERROR_S1_OPEN_CIRCUIT       0b00001000
/////////////////////////////////////////////////////
#define Sys_ERROR_S2_HIGH               0b0000000100000000
#define Sys_ERROR_S2_LOW                000000001000000000
#define Sys_ERROR_S2_SHORT_CIRCUIT      0b0000100000000000
#define Sys_ERROR_S2_OPEN_CIRCUIT       0b0001000000000000
/////////////////////////////////////////////////////
#define Sys_ERROR_CURRENT_HIGH           0b000000010000000000000000
#define Sys_ERROR_CURRENT_LOW            00000000100000000000000000
#define Sys_ERROR_CURRENT_SHORT_CIRCUIT  0b000010000000000000000000
#define Sys_ERROR_CURRENT_OPEN_CIRCUIT   0b000100000000000000000000
/////////////////////////////////////////////////////

//#endif /*__RECIRCULADOR__*/
#endif /* BOARD_DEFS_H*/