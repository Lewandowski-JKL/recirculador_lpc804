//Verificação caso a biblioteca ja tenha sido incluida
#ifndef ADC_H 
#define ADC_H
//
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "board_defs.h"

#ifdef __LPC804__
    #include "LPC804.h"
#endif

//Estrutura utilizada para configuração do ADC
typedef struct adc_t
{
    unsigned char _channel; //Canal utilizado pelo ADC
    unsigned short _measureLength; //Quantidade de valores utilizados para fazer a media
    long int _convertTo_mv; //Conversão para mili volts
}adc_t;
//Leitura do adc
int adc_Read(adc_t *adc);
//Leitura em MiliVolts
int adc_ReadMiliVolts(adc_t *adc);
int adc_ConvertToMiliVolts(adc_t *adc, int measure);
//Inicialização do periférico do ADC
void adc_Begin();
//Configuração do ADC
void adc_Config(adc_t *adc, unsigned char channel, unsigned short measureLength);

#endif