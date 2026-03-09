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


unsigned char getIntMessage[] =     {0x03, 0xEC, 0x13, 0x0B, 0x00};
unsigned char getFloatMessage[] =   {0x03, 0xBC, 0x1B, 0x07, 0x00};
unsigned char setPumpMessage[] =    {0x10, 0x49, 0x04, 0x01, 0x00, 0x01, 0xFF}; 


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
    gpio_ClearPin(Syspin_Pump);//Coloca ele em estado '0' | Desligado
    gpio_ConfigPinInput(Syspin_Botoeira);//Define o pino como entrada para leitura da botoeira
    gpio_ConfigPinInput(Syspin_SensorDeFluxo);//Define o pino como entrada para leitura do sensor de fluxo
    //Inicia o adc
    adc_Config(&adc_S1, SysADC_TempS1, SysADCMeasureLenght);
    adc_Config(&adc_S2, SysADC_TempS2, SysADCMeasureLenght);
    adc_Config(&adc_Vref, SysADC_VRef, SysADCMeasureLenght);

    //inicia iap flash ("eeprom")
    iap_Begin();
    // //Inicia valores dos registradores
    reg_Begin();//Fazer a inicialização depois
    
    // //inicia valores de acordo com a eeprom
    //initMyEEPROM();

    //defina a leitura do thermistor
    //Configura o termistor e o circuito de medidas
    thermistor_new(&temp_S1, 10000, 3300, 3300, -55, 125, NTC_10K_3380_VET);
    thermistor_new(&temp_S2, 10000, 3300, 3300, -55, 125, NTC_10K_3380_VET);

    //SysTickBeginISR(SysTickFrequency, isrSysTick);//Cria a interrupção para contagem de tempo e controle do sistema
    // //inicia interrupções
    // newExternInterrupt(Syspin_Botoeira,isrBotoeira,FALLING);//Cria a interrupção para leitura da botoeira
    // newExternInterrupt(Syspin_SensorDeFluxo,isrFlow,FALLING);//Cria a interrpção para leitura do fluxo

    //initQueue();//Inicia a fila de mensagens
    //inicia I2C
    i2cBegin(SysI2CADDR_Recirculador, Syspin_SDA, Syspin_SCL, SysI2CBaudRate, i2cModeMaster);

}
/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    unsigned int counter = 0;

    while (1)
    {
        getIntMessage[4] = counter%0xFF;
        getIntMessage[3] = (counter%0xFFFF)/0xFF;
        getIntMessage[2] = counter/0xFFFF;
        counter ++;
        i2cSend_master(SysI2CADDR_WiFi, getIntMessage, 5);
        task_delay_ms(2000);
    }
}


 // int main(void)
// {
//     unsigned char bufferRX[256];
//     unsigned int lasTimeSaveEeprom = 0;
//     unsigned char MessageStateControl = 0b10000000;
//     unsigned int timeAuxMessage = 0;
//     unsigned int flagMessage = 1;
//     unsigned int lastTimeAddMessage = 0;
//     while (1)
//     {
//         if (ptrPriorityMessage->len)
//             ControlMessageFunc(&ptrPriorityMessage, bufferRX, &MessageStateControl, &timeAuxMessage);
//             else if (ptrMessage->len)
//                ControlMessageFunc(&ptrMessage, bufferRX, &MessageStateControl, &timeAuxMessage);
//         if ((SysTickGetTicks()-lasTimeSaveEeprom)>SysTicks100_ms)
//         {
//             lasTimeSaveEeprom = SysTickGetTicks();
//             _writeRegistersInEEPROM();
//         }
        
//         if ((SysTickGetTicks()-lastTimeAddMessage)>SysTicks200_ms)//100ms
//         {
//             lastTimeAddMessage = SysTickGetTicks();
//             switch (flagMessage)
//             {
//             case 0b00000001:
//                 addNewMessage(ptrMessage, WIFI_ADDR, (unsigned char*)getTypeMessage, 
//                                         sizeof(getTypeMessage), 82, receiveType);
//                 flagMessage = 0b00000010;
//                 break;
//             case 0b00000010:
//                 flagMessage = 0b00000100;
//                 addNewMessage(ptrMessage, WIFI_ADDR, (unsigned char*)getBoolMessage, 
//                                     sizeof(getBoolMessage), 5, checkBoolReg);
//                 break;
//             case 0b00000100:
//                 flagMessage = 0b00001000;
//                 addNewMessage(ptrMessage, WIFI_ADDR, (unsigned char*)getIntMessage, 
//                                     sizeof(getIntMessage), 48, checkIntReg);
//                 break;
//             case 0b00001000:
//                 flagMessage = 0b00000001;
//                 addNewMessage(ptrMessage, WIFI_ADDR, (unsigned char*)getFloatMessage, 
//                                     sizeof(getFloatMessage), 16, checkFloatReg);
//                 break;
//             default:
//                 flagMessage = 0b00000001;
//                 break;
//             }
//         }
//     }
// }
