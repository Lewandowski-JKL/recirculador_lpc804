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
#include "rtc.h"



/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void setup()   __attribute__ ((constructor));
void setup()
{
    //Defin e o CLock do microcontrolador para 30Mhz
    SysConfigClock30M();
    
    //Inicia pinos que se interligam com o modulo Wifi
    gpio_ConfigPinOutput(Syspin_nReload);//Define o pino como saída
    gpio_ClearPin(Syspin_nReload);//Coloca ele em estado '0'
    gpio_ConfigPinOutput(Syspin_RESET);//Define pino como saída
    gpio_ClearPin(Syspin_RESET);//Coloca ele em estado '0'
    gpio_ConfigPinInput(Syspin_nReady);//Define pino como entrada
    gpio_ConfigPinInput(Syspin_nLink);//Define pino como entrada

    //Inicia Pinos utilizados para controlar o equipamento
    gpio_ConfigPinOutput(Syspin_Pump);//Define o pino utilizado para ligar a bomba como Saída
    reg_write_bool(false, Sys_RegMap_Pump);
    gpio_SetPin(Syspin_Pump);//Coloca ele em estado '1' | Desligado
    gpio_ConfigPinInput(Syspin_Botoeira);//Define o pino como entrada para leitura da botoeira
    gpio_ConfigPinInput(Syspin_SensorDeFluxo);//Define o pino como entrada para leitura do sensor de fluxo

    //Inicia o adc
    adc_Config(&adc_S1, SysADC_TempS1, SysADCMeasureLenght);
    adc_Config(&adc_S2, SysADC_TempS2, SysADCMeasureLenght);
    adc_Config(&adc_Vref, SysADC_VRef, SysADCMeasureLenght);
    adc_Config(&adc_Current, SysADC_Current, SysADCMeasureLenght);

    //inicia iap flash ("eeprom")
    //Inicia valores dos registradores
    reg_Begin();//Fazer a inicialização depois
    
    //defina a leitura do thermistor
    //Configura o termistor e o circuito de medidas
    thermistor_new(&temp_S1, 10000, 3300, 2500, -55, 125, NTC_10K_3380_VET);
    thermistor_new(&temp_S2, 10000, 3300, 2500, -55, 125, NTC_10K_3380_VET);
    //inicia interrupções
    //Para leitura da botoeira
    newExternInterrupt(Syspin_Botoeira, rec_isr_Botoeira, FALLING, NULL);
    //Cria a interrução para leitura do fluxo
    newExternInterrupt(Syspin_SensorDeFluxo, rec_isr_Flow, FALLING, NULL);
    //Inicia a fila de mensagens
    message_Init();
    //inicia I2C
    i2cBegin(SysI2C_ADDR_Recirculador, Syspin_SDA, Syspin_SCL, SysI2CBaudRate, i2cModeMaster);
    //Inicia as tasks
    task_new(rec_get_Temp, "Rec_Get_Temp", 3, 0, NULL);//Faz 10 medidas de temperatura por segundo
    task_new(rec_measure, "measure_Process", 1, 0, NULL);//Faz a media das ultimas medidas e adiciona no registrador
    task_new(rec_current_measure, "current_measure", 5,0, NULL);
    task_new(rec_ref_measure, "ref_measure", 1,0, NULL);
    task_new(rec_system, "System", 20, 0, NULL);   //Processa mudanças e, caso perceba alguma,
                                                    // cria a mensagem para ser enviada para o modulo wifi e o display
    task_new(rec_error_process, "Error_Process", 1, 0, NULL);//identifica erros e cria a mensagem de erro
    /////// -> le o valor no RTC
    pontingTimestamp((unsigned int *)reg_ptr_int(Sys_RegMap_Timestamp));
    task_new(calendar_tickTimestamp, "tickTimestamp", 1, 0, NULL);//Contador do timestamp a cada 1s
    ////////////////
    task_delay_ms(4000); //Espera 4 segundos antes de começar a criar as mensagens
    task_new(rec_change_verify, "reg_change", 5, 0, NULL);
    task_new(rec_create_messages, "create_messages", 4,0, NULL);
    //task_new(rec_eeprom_test, "eeprom_test", 1, 0, NULL);//Contador do timestamp a cada 1s
    //Atualiza todos os registradores
    task_delay_ms(1000);
    // Remove a opção de reset do botão
    //gpio_resetDisable();
    //gpio_ConfigPinInput(Syspin_RESET);//Define o pino como entrada para leitura do sensor de fluxo
}

int main(void)
{
    while (1)
    {
        message_loop();
    }
}
