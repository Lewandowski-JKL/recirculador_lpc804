#include "message_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

#ifndef Sys_Modbus_Message_Queue_Size
    #define Sys_Modbus_Message_Queue_Size 5
#endif
#ifndef Sys_Modbus_Time_Reply
    #define Sys_Modbus_Time_Reply 50
#endif


#define Sys_Modbus_Message_Flag_Send        0b0000000000000011
#define Sys_Modbus_Message_Flag_Send_Fail   0b0000000000000100
#define Sys_Modbus_Message_Flag_Wait_Reply  0b0000000000001000
#define Sys_Modbus_Message_Flag_Read_Ewply  0b0000000000010000

unsigned short message_stage_flag = 1;


i2c_modbus_s  _message_default =
{           
    .code=0,
    .RegAddr=0,
    .nReg=0,
    .bytes=0,
    .ptrMessage=NULL,
    .crc=0,
    .addr=0,
    .size_head=0,
    .size_message=0    
}; 

typedef struct message_s
{
    i2c_modbus_s message;
    void *ptrNext;
}message_s;

message_s _Queue[Sys_Modbus_Message_Queue_Size];
message_s *_ptrQueue;

int _message_init = 0;

void message_Init()
{
    for (int i = 0; i < Sys_Modbus_Message_Queue_Size; i++)
    {
        _Queue[i].message = _message_default;
        _Queue[i].ptrNext = (void*)&_Queue[(i+1)%Sys_Modbus_Message_Queue_Size];
    }
    _ptrQueue = _Queue;
    _message_init = 1;
}

void message_Delete()
{
    if (!_message_init)
        return;

}
int message_QueueLen()
{
    if (!_message_init)
        return 0;
    return 0;
}
int message_QueueSize()
{
    if (!_message_init)
        return 0;
    return 0;
}
int message_New(i2c_modbus_s newMessage)
{
    if (!_message_init)
        return 0;
    int counter=0;
    message_s *ptrAux = _ptrQueue;
    while (ptrAux->message.ptrMessage != NULL)
    {
        counter++;
        if (counter == Sys_Modbus_Message_Queue_Size)
            return Message_QueueOverload;
    }
    ptrAux->message = newMessage;
    return Message_Ok;
}

/*******************************************************************************
 * Loop de gestão das Mensagens
 *****************************************************************************/
void message_loop()
{
    if (!_message_init)
        return;
    if (_ptrQueue->message.ptrMessage == NULL)
    {
        _ptrQueue = (message_s*)_ptrQueue->ptrNext;
        return;
    }
    //Verifica se esta esperando uma resposta
    task_scheduler_pause();
    i2cSend_master_modbus(&_ptrQueue->message);
    task_scheduler_continue();
    message_stage_flag++;
    message_stage_flag |= Sys_Modbus_Message_Flag_Wait_Reply;
    task_delay_ms(10);
    //esperar retorno entre outras coisas
    //tratar mensagem e recebimento
    _ptrQueue->message = _message_default;
    _ptrQueue = (message_s*)_ptrQueue->ptrNext;
}
/*
void ControlMessageFunc(myMessage **ptr, unsigned char *ptrRX, unsigned char *MessageStateControl, unsigned int *ptrTime)
{   
    if (*MessageStateControl & 0b1000)//Falhou 8 vezes, então exclui a mensagem
    {
        //blink = !blink; -> utilizado para auxilio de debug no passado
        deleteMessage(ptr);//Deleta a mensagem
        *MessageStateControl = 0b100000;//Coloca em estado de espera
    }
    if (*MessageStateControl & 0b10000000)//estado de enviar mensagem
    {
        if((*ptr)->len == 0)
            return;
        if (sendI2C_master((*ptr)->addr, (*ptr)->message, (*ptr)->len) == i2cStat_OK)
        {
            *ptrTime = SysTickGetTime_ms();
            *MessageStateControl |= 0b1000000;//entra no estado de espera para leitura
            *MessageStateControl &= 0b1001111;//filtra para a contagem de mensagens
        }else
            *MessageStateControl = *MessageStateControl+1;
    }else if (*MessageStateControl & 0b1000000)//estado de espera para leitura
        {
            if ((SysTickGetTime_ms()-*ptrTime)<50)//ainda em estado de espera até passar 50 ms
                return;
            readLenI2C_master((*ptr)->addr, ptrRX, (*ptr)->replyLen);
            if (CHECK_CRC16(ptrRX, (*ptr)->replyLen))
            {
                *ptrTime = SysTickGetTime_ms();
                *MessageStateControl = 0b100000; //espera mais 5ms para enviar a proxima mensagem
                if((*ptr)->ptrFunc != NULL)
                    (*ptr)->ptrFunc(ptrRX);
                //blink = !blink;
                deleteMessage(ptr);
                return;
            }
            readLenI2C_master((*ptr)->addr, ptrRX,  0xFF);//limpa o buffer de leitura
            (*MessageStateControl)++;//add erro
            *MessageStateControl |= 0b10000000;//reenvia a mensagem
            *MessageStateControl &= 0b10001111;//filtra para a contagem de mensagens
        }else if (*MessageStateControl & 0b100000)//
            {
                if ((SysTickGetTime_ms()-*ptrTime)<5)//ainda em estado de espera até passar 5 ms
                    return;
                *MessageStateControl = 0b10000000;//preparado para enviar uma nova mensagem
            }else if (*MessageStateControl & 0b10000)//não utilizado
                {
                    *MessageStateControl = 0b10000000;
                }
}
*/
