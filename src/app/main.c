/**
 * @file main.c
 * @author Jackson Kenedi Lewandowski  (jackson.lewandowski@komlog.com/jacksonlewandowski.work@gmail.com)
 * @brief Codigo que controla o sistema de recirculação
 * @version 0.1
 * @date 2024-11-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include <time.h>
#include <math.h>

//my libs
#include "system.h"
#include "app_config.h"
#include "message_manager.h"
#include "registers_manager.h"
#include "recirculador.h"
#include "task.h"
#include "flash_map.h"
#include "checksum.h"


// unsigned char getIntMessage[] =     {0x03, 0xEC, 0x13, 0x0B, 0x00};
// unsigned char getFloatMessage[] =   {0x03, 0xBC, 0x1B, 0x07, 0x00};
// unsigned char setPumpMessage[] =    {0x10, 0x49, 0x04, 0x01, 0x00, 0x01, 0xFF}; 

void debug_test(void *arg);
//#define TESTE
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void setup()   __attribute__ ((constructor));

void setup()
{
    SysConfigClock30M(); //Defin e o CLock do microcontrolador para 30Mhz
    //Inicia pinos que se interligam com o modulo
    gpio_ConfigPinOutput(Syspin_nReload);//Define o pino como saída
    gpio_ClearPin(Syspin_nReload);//Coloca ele em estado '0'
    gpio_ConfigPinOutput(Syspin_RESET);//Define pino como saída
    gpio_ClearPin(Syspin_RESET);//Coloca ele em estado '0'
    gpio_ConfigPinInput(Syspin_nReady);//Define pino como entrada
    gpio_ConfigPinInput(Syspin_nLink);//Define pino como entrada

    //Inicia Pinos utilizados para controlar o equipamento
    gpio_ConfigPinOutput(Syspin_Pump);//Define o pino utilizado para ligar a bomba como Saída
    reg_write_bool(false, Sys_RegMap_Pump);
    gpio_SetPin(Syspin_Pump);
    //gpio_SetPin(Syspin_Pump);//Coloca ele em estado '1' | Desligado
    gpio_ConfigPinInput(Syspin_Botoeira);//Define o pino como entrada para leitura da botoeira
    gpio_ConfigPinInput(Syspin_SensorDeFluxo);//Define o pino como entrada para leitura do sensor de fluxo
    //Inicia o adc
    adc_Config(&adc_S1, SysADC_TempS1, SysADCMeasureLenght);
    adc_Config(&adc_S2, SysADC_TempS2, SysADCMeasureLenght);
    adc_Config(&adc_Vref, SysADC_VRef, SysADCMeasureLenght);
    // adc_Config(&adc_Current, SysADC_Current, SysADCMeasureLenght);

    //inicia iap flash ("eeprom")
    // //Inicia valores dos registradores
    reg_Begin();//Fazer a inicialização depois
    
    //defina a leitura do thermistor
    //Configura o termistor e o circuito de medidas
    thermistor_new(&temp_S1, 10000, 3300, 2500, -55, 125, NTC_10K_3380_VET);
    thermistor_new(&temp_S2, 10000, 3300, 2500, -55, 125, NTC_10K_3380_VET);

    //Inicia as tasks
    task_new(rec_get_Temp1, "Rec_Get_Temp_1", 10, 0, NULL);//Faz 10 medidas de temperatura por segundo
    task_new(rec_get_Temp2, "Rec_Get_Temp_2", 10, 0, NULL);//Faz 10 medidas de temperatura por segundo
    task_new(rec_system, "System", 100, 0, NULL);   //Processa mudanças e, caso perceba alguma,
                                                    // cria a mensagem para ser enviada para o modulo wifi e o display
    task_new(rec_error_process, "Error_Process", 1, 0, NULL);//identifica erros e cria a mensagem de erro
    task_new(rec_measure, "measure_Process", 1, 0, NULL);//Faz a media das ultimas medidas e adiciona no registrador
    task_new(rec_change_verify, "reg_change", 10, 0, NULL);
    //task_new(debug_test, "debug_test", 1, 0, NULL);


    //inicia interrupções
    //Para leitura da botoeira
    newExternInterrupt(Syspin_Botoeira, rec_isr_Botoeira, FALLING, NULL);
    //Cria a interrução para leitura do fluxo
    newExternInterrupt(Syspin_SensorDeFluxo, rec_isr_Flow, FALLING, NULL);

    message_Init();//Inicia a fila de mensagens
    //inicia I2C
    i2cBegin(SysI2CADDR_Recirculador, Syspin_SDA, Syspin_SCL, SysI2CBaudRate, i2cModeMaster);
    
    unsigned int time=0;
    task_delay_ms_until(&time, 1000);
    //Remover o reset pelo pino
}
/**
 * @brief 
 * 
 * @return int 
 */
typedef struct debug_test_s
{
    int counter;
}debug_test_s;
debug_test_s debug_test_var =
{
    .counter = 0
};

void debug_test(void *arg)
{
    debug_test_var.counter++;
    debug_test_var.counter %= 5;
    if (debug_test_var.counter)
        return;
    
    i2c_modbus_s message;

    message.code = Sys_Modbus_Set_Mult_Num_Reg;
    message.RegAddr = Sys_RegMap_Offset_Int;
    message.nReg = 10;
    message.bytes = 40;
    message.ptrMessage = reg_ptr();
    message.addr = SysI2CADDR_WiFi;
    message.size_head = 5;
    message.size_message = message.bytes;

    message_New(message);
}

int main(void)
{           
    unsigned long long last_time_att_timestamp = 0;
    while (1)
    {
        if ((SysTickGetTime_ms() - last_time_att_timestamp) >= 1000)//1000ms
        {
            if (reg_return_change_flag() & Reg_Change_Int_Register)
            {
                reg_write_int(reg_read_int(Sys_RegMap_Timestamp)+1, Sys_RegMap_Timestamp);
            }else
            {
                task_scheduler_pause();
                reg_write_int(reg_read_int(Sys_RegMap_Timestamp)+1, Sys_RegMap_Timestamp);
                reg_clear_change_flag(Reg_Change_Int_Register);
                task_scheduler_continue();
            }
            last_time_att_timestamp = SysTickGetTime_ms();
        }
        message_loop();
    }
}
