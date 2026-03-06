#include <stdio.h>
#include "board_defs.h"
#include "recirculador.h"
#include "system.h"
#include "app_config.h"
#include "message_manager.h"
#include "registers_manager.h"


void rec_isrBotoeira(volatile void *arg)
{
    // if ((SysTickGetTime_ms() - timeDebounceBotoeira) < (20*SysTicksButtonDebounce))//faz o teste de debounce para evitar ruido 100ms
    //     return;
    // if(gpio_ReadPin(Syspin_Botoeira))//verifica se o botão continua pressionado 
    //     return;
    // timeDebounceBotoeira = SysTickGetTime_ms();//Salva tempo atual para uso no proximo teste de debounce
    // setBoolValue(!getBoolWithAddr(regMapBotoeira),regMapBotoeira);//Altera o valor da botoeira
}

/**
 * @brief Tratamento da interrupção da entrada de sensor de fluxo
 * 
 * @param arg 
 */
void isrFlow(volatile void *arg)
{
    // // Nãu usa tratamento de debounce para evitar erro na leitura
    // // if ((SysTickGetTime_ms() - timeDebounceFlow) < timeDebounce_ms)
    // //     return;
    // if(gpio_ReadPin(Syspin_SensorDeFluxo))//Auxilia como um filtro de ruido
    //     return;
    // //timeDebounceFlow = SysTickGetTime_ms();
    // FlowCounter++;//Incrementa contador de fluxo
}
/**
 * @brief Função que trata interrupção do timer 0
 *      Não utilizada
 * 
 * @param arg 
 */
void isrTimer0(volatile void *arg)
{
    
}

/***********************
 * Variaveis Sys
***********************/
/**
 * Essa função faz o o tratamento da interrupção do sistema que ocorre a cada 100us
*/
void isrSysTick(volatile void *arg)
{
    /*if (flagReceiveMessage)
        return;*/
    // unsigned int SysTicksGetAux = SysTickGetTicks();
    /*if(SysTicksGetAux % SysTicksToProcess)
        return;*/
    // if(!(SysTicksGetAux % SysTicks100_ms))//Faz a leitura da temperatura a cada 100ms
    //     getTemp();
    // if(!(SysTicksGetAux % SysTicks1000_ms))//Faz a leitura do fluxo a ca 1s
    // {
    //     tickTimestamp();
    //     //FlowCounter *= getFloatWithAddr(regMapCalibFluxo);
    //     //setFloatValue(FlowCounter, regMapCalibFluxo);
    //     FlowCounter = 0;
    // }
    // if(!(SysTicksGetAux % SysTicks10_ms))//Processa possíveis alterações do sistema a cada 10ms
    //     processFunc();
    /*if(!(SysTicksGetAux % SysTicks100_ms))
        _writeRegistersInEEPROM();//atualiza eeprom (so faz commit se teve alguma alteração)*/
}



// typedef struct getTemp_s
// {
//     /* data */
// }getTemp_s;
// getTemp_s getTemp_var;
void getTemp()
{
    //Faz a leitura do adc, converte em mV apos isso converte em °C
    //setFloatValue(convert_mV_to_celcius(&temperature, adc_ReadMiliVolts(&temperatureADC)), regMapTemperaturaMedida);
    //setFloatValue((readMiliVolts(&temperatureADC)/10), regMapFluxo);
    //setFloatValue(2000, regMapTemperaturaMedida);
}
void processFunc()
{
    // short tempSup = getFloatWithAddr(regMapTemperaturaRef) + 100;//Temperatura referencia + 1°C
    // short tempInf = getFloatWithAddr(regMapTemperaturaRef) - 100;//Temperatura referencia - 1°C
    // short temp = getFloatWithAddr(regMapTemperaturaMedida);//Temperatura atual
    // //Verifica se ja passou o tempo de recirculação de 30s
    // if(((SysTickGetTime_ms() - timeDebounceBotoeira) > SysRecirculationTicks) && getBoolWithAddr(regMapBotoeira))
    //     setBoolValue(false,regMapBotoeira);
    
    // if (schedulingTest() || getBoolWithAddr(regMapBotoeira))//Verifica se tem algum agendamento ou se a botoeira esta pressionada
    // {
    //     if (tempInf>temp)//Verifica se a temperatura atual está abaixo da temperatura minima
    //     {
    //         if (!getBoolWithAddr(regMapBomba))//Caso a bomba está desligada é enviado uma mensagem informando que a bomba foi ligada
    //             addNewMessage(ptrPriorityMessage, WIFI_ADDR, (unsigned char*)setPumpMessage, 
    //                     sizeof(setPumpMessage), sizeof(setPumpMessage)+2, NULL);
    //         setBoolValue(true,regMapBomba);//Escreve no registrador que a bomba está ligada
    //         gpio_SetPin(Syspin_Pump);//Liga a bomba
    //     }else if(tempSup < temp)//Verifica se a temperatura estáacima da temperatura máxima
    //         {
    //             if (getBoolWithAddr(regMapBomba))//Verifica se a bomba está ligada, caso esteja informa que a bomba será desligada
    //                 addNewMessage(ptrPriorityMessage, WIFI_ADDR, (unsigned char*)resetPumpMessage, 
    //                         sizeof(resetPumpMessage), sizeof(resetPumpMessage)+2, NULL);
    //             setBoolValue(false,regMapBomba);//Define o registrador da bomba como off
    //             gpio_ClearPin(Syspin_Pump);//Desliga a bomba
    //         }
    // }else
    //     {
    //         if (getBoolWithAddr(regMapBomba))//Verifica se a bomba está ligada, caso esteja informa que a bomba será desligada
    //             addNewMessage(ptrPriorityMessage, WIFI_ADDR, (unsigned char*)resetPumpMessage, 
    //                      sizeof(resetPumpMessage), sizeof(resetPumpMessage)+2, NULL);
    //         setBoolValue(false,regMapBomba);//Define o registrador da bomba como off
    //         gpio_ClearPin(Syspin_Pump);//Desliga a bomba
    //     }
}
/**
 * @brief Salva registradores na memória FLASH
 * 
 */
void _writeRegistersInEEPROM()
{
    // unsigned char vet[IAP_FLASH_SIZE] = {0};
    // vet[IAP_FLASH_SIZE-1] = 0x01;
    // //salva do endereço mac até a senha da rede
    // unsigned char *ptrAux = vet;
    // unsigned short addrAux = (regMapEnderecoMacInit-regMapShortOffset-1);
    // unsigned char sizeAux = ((regMapSenhaDaRedeEnd-regMapEnderecoMacInit+1)*sizeof(short));//70;
    // memcpy(ptrAux,(void*)&ShortRegisters[addrAux],sizeAux);
    // //salva agendamentos
    // ptrAux = (ptrAux+sizeAux);//(ptrAux+sizeAux);//anda o ponteiro aux
    // addrAux = (regMapAgendamentosInit-regMapIntOffset-1);
    // sizeAux = (regMapAgendamentosEnd-regMapAgendamentosInit+1)*sizeof(int);
    // memcpy(ptrAux,(void*)&IntRegisters[addrAux],sizeAux);
    // //salva valores float (calibração e medidas)
    // ptrAux = (ptrAux+sizeAux);//anda o ponteiro aux
    // addrAux = (regMapLitrosTotais-regMapFloatOffset-1);
    // sizeAux = ((regMapCalibFluxo-regMapLitrosTotais+1)*sizeof(short));
    // memcpy(ptrAux,(void*)&FloatRegisters[addrAux],sizeAux);
    // iap_writeVet(vet,IAP_FLASH_SIZE,0);
    // iap_commit();
}
/**
 * @brief Carrega valores presentes na flash para o s registradores
 * 
 */
void _writeEEPROMvaluesInRegisters()
{
    // unsigned char vet[IAP_FLASH_SIZE] = {0};
    // iap_readVet(vet, IAP_FLASH_SIZE, 0);
    // //carrega mac, nome da rede e senha
    // unsigned char *ptrAux = vet;
    // unsigned short addrAux = (regMapEnderecoMacInit-regMapShortOffset-1);
    // unsigned char sizeAux = (regMapSenhaDaRedeEnd-regMapEnderecoMacInit+1)*sizeof(short);//70;
    // memcpy((void*)&ShortRegisters[addrAux],ptrAux,sizeAux);
    // //carrega agendamentos
    // ptrAux = (ptrAux+sizeAux);//anda o ponteiro aux
    // addrAux = (regMapAgendamentosInit-regMapIntOffset-1);
    // sizeAux = (regMapAgendamentosEnd-regMapAgendamentosInit+1)*sizeof(int);
    // memcpy((void*)&IntRegisters[addrAux],ptrAux,sizeAux);
    // //carrega valores float
    // ptrAux = (ptrAux+sizeAux);//anda o ponteiro aux
    // addrAux = (regMapLitrosTotais-regMapFloatOffset-1);
    // sizeAux = (regMapCalibFluxo-regMapLitrosTotais+1)*sizeof(short);
    // memcpy((void*)&FloatRegisters[addrAux],ptrAux,sizeAux);
}
/**
 * @brief Inicia a "EEPROM"
 * 
 */
void initMyEEPROM()
{   
    // if (((unsigned char)0b01) == iap_ReadChar(IAP_FLASH_SIZE-1))//ja iniciado
    // {
    //     _writeEEPROMvaluesInRegisters();
    //     return;
    // }
    // _writeRegistersInEEPROM();
}
/*******************************************************************************
 * Receive Function
 ******************************************************************************/
/**
 * Inicia a fila de mensagens
*/
void initQueue()
{
    // ptrPriorityMessage = &priorityMessage;
    // ptrPriorityMessage->ptrNext = ptrPriorityMessage;
    // ptrPriorityMessage->addr = 0;
    // ptrPriorityMessage->len = 0;
    // ptrPriorityMessage->replyLen = 0;
    // ptrPriorityMessage->ptrFunc = NULL;

    // for (int i = 0; i < LengthMessageQueue; i++)
    // {
    //     messageQueue[i].ptrNext = &messageQueue[(i+1)%LengthMessageQueue];
    //     messageQueue[i].addr = 0;
    //     messageQueue[i].len = 0;
    //     messageQueue[i].ptrFunc = NULL;
    //     messageQueue[i].replyLen = 0;
    // }
    // ptrMessage = &messageQueue[0];
}
/**
 * @brief Faz a gestão das mensagens enviadas
 * 
 * @param ptr ponteiro para objeto com as informações da mensagem que será enviada
 * @param ptrRX ponteiro para buffer que irá receber a resposta
 * @param MessageStateControl flag de auxilio para saber em que etapa de processamento ela esta
 * @param ptrTime tempo para auxílio no controle da mensagem
 * 
 * Alguns parametros não foram acrescentados no objeto para impedir de alocar muita memoria visto que o microcontrolador não aceitou muito bem esse processo
 */
void ControlMessageFunc(myMessage **ptr, unsigned char *ptrRX, unsigned char *MessageStateControl, unsigned int *ptrTime)
{   
    // if (*MessageStateControl & 0b1000)//Falhou 8 vezes, então exclui a mensagem
    // {
    //     //blink = !blink; -> utilizado para auxilio de debug no passado
    //     deleteMessage(ptr);//Deleta a mensagem
    //     *MessageStateControl = 0b100000;//Coloca em estado de espera
    // }
    // if (*MessageStateControl & 0b10000000)//estado de enviar mensagem
    // {
    //     if((*ptr)->len == 0)
    //         return;
    //     if (i2cSend_master((*ptr)->addr, (*ptr)->message, (*ptr)->len) == i2cStat_OK)
    //     {
    //         *ptrTime = SysTickGetTime_ms();
    //         *MessageStateControl |= 0b1000000;//entra no estado de espera para leitura
    //         *MessageStateControl &= 0b1001111;//filtra para a contagem de mensagens
    //     }else
    //         *MessageStateControl = *MessageStateControl+1;
    // }else if (*MessageStateControl & 0b1000000)//estado de espera para leitura
    //     {
    //         if ((SysTickGetTime_ms()-*ptrTime)<50)//ainda em estado de espera até passar 50 ms
    //             return;
    //         i2cReadLenght_master((*ptr)->addr, ptrRX, (*ptr)->replyLen);
    //         if (CHECK_CRC16(ptrRX, (*ptr)->replyLen))
    //         {
    //             *ptrTime = SysTickGetTime_ms();
    //             *MessageStateControl = 0b100000; //espera mais 5ms para enviar a proxima mensagem
    //             if((*ptr)->ptrFunc != NULL)
    //                 (*ptr)->ptrFunc(ptrRX);
    //             //blink = !blink;
    //             deleteMessage(ptr);
    //             return;
    //         }
    //         i2cReadLenght_master((*ptr)->addr, ptrRX,  0xFF);//limpa o buffer de leitura
    //         (*MessageStateControl)++;//add erro
    //         *MessageStateControl |= 0b10000000;//reenvia a mensagem
    //         *MessageStateControl &= 0b10001111;//filtra para a contagem de mensagens
    //     }else if (*MessageStateControl & 0b100000)//
    //         {
    //             if ((SysTickGetTime_ms()-*ptrTime)<5)//ainda em estado de espera até passar 5 ms
    //                 return;
    //             *MessageStateControl = 0b10000000;//preparado para enviar uma nova mensagem
    //         }else if (*MessageStateControl & 0b10000)//não utilizado
    //             {
    //                 *MessageStateControl = 0b10000000;
    //             }
}
/**
 * @brief processa a resposta de 'getType'
 * 
 * @param data 
 */
void receiveType(unsigned char *data)
{
    //blink = !blink;
    // memcpy((void*)LastShortRegisters, (void*)&data[3], sizeof(ShortRegisters));
    // unsigned char dataAux[sizeof(ShortRegisters)+8];
    // unsigned char sizeAux = compareShortRegister(dataAux);
    // if(sizeAux)
    //     addNewMessage(ptrMessage, WIFI_ADDR, dataAux, sizeAux, sizeAux+2, NULL);
}
/**
 * @brief trata o pacote de mensagens que se refere a variaveis booleanas
 * 
 * @param data 
 */
void checkBoolReg(unsigned char *data)
{
    /*blink = !blink;*/
    // for (int i = 0; i < sizeof(BoolRegisters); i++)
    //     LastBoolRegisters[i] = data[3+(i/8)] & (0b10000000 >> (i%8));
    // unsigned char dataAux[9+(sizeof(BoolRegisters)/8)];
    // unsigned char sizeAux = compareBoolRegister(dataAux);
    // if(sizeAux)
    //     addNewMessage(ptrMessage, WIFI_ADDR, dataAux, sizeAux, sizeAux+2, NULL);
}
/**
 * @brief Trata o pacote de mensagens que se refere as variaveis short -> não utilizado
 * 
 * @param data 
 */
void checkShortReg(unsigned char *data){}
/**
 * @brief Trata a mensagem que se refere ao pacote de registradores int
 * 
 * @param data 
 */
void checkIntReg(unsigned char *data)
{
    //blink = !blink;
    // memcpy((void*)LastIntRegisters, (void*)&data[3], sizeof(IntRegisters));
    // unsigned char dataAux[sizeof(IntRegisters)+8];
    // unsigned char sizeAux = compareIntRegister(dataAux);
    // if(sizeAux)
    //     addNewMessage(ptrMessage, WIFI_ADDR, dataAux, sizeAux, sizeAux+2, NULL);
}
/**
 * @brief Trata a mensagem que se refere ao pacote de registradores float
 * 
 * @param data 
 */
void checkFloatReg(unsigned char *data)
{
    // memcpy((void*)LastFloatRegisters, (void*)&data[2], sizeof(FloatRegisters));
    // unsigned char dataAux[sizeof(FloatRegisters)+8];
    // unsigned char sizeAux = compareFloatRegister(dataAux);
    // if(sizeAux)
    //     addNewMessage(ptrMessage, WIFI_ADDR, dataAux, sizeAux, sizeAux+2, NULL);
}
/**
 * @brief Faz o calculo de log de 2 do valor enviado
 * 
 * @param value valor a ser calculado o log
 * @return int retorno
 */
int myLog2(unsigned int value)
{
    int i = 0;
    while (value)
    {
        value/=2;
        i++;
    }
    return i;
}