#include "message_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <task.h>
#include "checksum.h"
#include "registers_manager.h"
#include "interrupt.h"

#ifndef Sys_Modbus_Message_Queue_Size
    #define Sys_Modbus_Message_Queue_Size 5
#endif
#ifndef Sys_Modbus_Time_Reply
    #define Sys_Modbus_Time_Reply 50
#endif

#define Modbus_Get_Bool_Reg         (0x01)
#define Modbus_Get_Discrete_Input   (0x02)
#define Modbus_Get_Num_Reg          (0x03)
#define Modbus_Read_Input_Reg       (0x04)
#define Modbus_Set_Bool_Reg         (0x05)
#define Modbus_Set_Single_Reg       (0x06)
#define Modbus_Read_Exception       (0x07)
#define Modbus_Diagnostics          (0x08)
#define Modbus_Get_Event_Counter    (0x0B)
#define Modbus_Get_Event_Log        (0x0C)
#define Modbus_Set_Mult_Bool_Reg    (0x0F)
#define Modbus_Set_Mult_Num_Reg     (0x10)
#define Modbus_Report_Slave_ID      (0x11)
#define Modbus_Read_File_Record     (0x14)
#define Modbus_Write_File_Record    (0x15)
#define Modbus_Mask_Write_Register  (0x16)
#define Modbus_RW_Mult_Registers    (0x17)
#define Modbus_Read_FIFO_Queue      (0x18)
#define Modbus_Read_Device_ID       (0x2B)


#define Sys_Modbus_Message_Flag_Send        0b0000000000000011
#define Sys_Modbus_Message_Flag_Send_Fail   0b0000000000000100
#define Sys_Modbus_Message_Flag_Wait_Reply  0b0000000000001000
#define Sys_Modbus_Message_Flag_Read_Ewply  0b0000000000010000

enum replyStatus{
    _replyFail,
    _replyOK
};

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

int _message_queue_lenght = 0;
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
    //task_scheduler_pause();
    _message_queue_lenght++;
    if (_message_queue_lenght == Sys_Modbus_Message_Queue_Size)
    {
        _message_queue_lenght--;
        return Message_QueueOverload;
    }
    message_s *ptrAux = _ptrQueue;
    for (int i = 0; i < _message_queue_lenght; i++)
        ptrAux = (message_s *)ptrAux->ptrNext;
    ptrAux->message = newMessage;
    //task_scheduler_continue();
    return Message_Ok;
}

/*******************************************************************************
 * Loop de gestão das Mensagens
 *****************************************************************************/
int _Generic_Set_Reg(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 7 onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX 0xXX -> Endereço que será escrito
    //  0xXX 0xXX -> Quantidade de registradores escritos ou valor que será escrito caso seja apenas 1 reg;
    //  0xXX 0xXX -> CRC
    int sizeReply = 0;
    if (ptrAux->message.code == Modbus_Diagnostics)
        sizeReply = 5;//Loopback de diagnostico
    else
        sizeReply = 7;
    //Colocar outras condições especiais
    
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    sizeReply -= 2;
    for (int i = 0; i < sizeReply; i++)
        if (ptrAux->message.ptrMessage[i] != ptrReply[i])
            return _replyFail;
    return _replyOK;
}
int _Generic_Read_Reg(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4+nBytes onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX -> Numero de bytes (mínimo 1)
    //  0xXX 0x.. -> Data
    //  0xXX 0xXX -> CRC
    int nBytes = Sys_RegMap_GetRegBytes(ptrAux->message.RegAddr)*ptrAux->message.nReg;
    if (nBytes < 1)
        return _replyFail;
    int sizeReply = 4+nBytes;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    sizeReply -= 2;
    reg_write_vet(&ptrReply[2], ptrAux->message.nReg, ptrAux->message.RegAddr);
    return _replyOK;
}
int _Get_Bool_Reg(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4+nBytes onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX -> Numero de bytes (mínimo 1)
    //  0xXX 0x.. -> Data
    //  0xXX 0xXX -> CRC
    int sizeReply = 4 + (ptrAux->message.nReg/8);
    if (ptrAux->message.nReg%8)
        sizeReply++;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    int nReg = ptrAux->message.nReg;
    int addrAux = ptrAux->message.RegAddr;
    for (int i = 0; i < ptrReply[1]; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            reg_write_bool((bool)(ptrReply[2+i] & (0b10000000 >> j)), addrAux);
            nReg--;
            if (nReg == 0)
                return _replyOK;  
            addrAux++; 
        }
    }
    return _replyFail;
}
int _Get_Discrete_Input(message_s* ptrAux)
{
    return _Generic_Read_Reg(ptrAux);
}
int _Get_Num_Reg(message_s* ptrAux)
{
    return _Generic_Read_Reg(ptrAux);
}
int _Read_Input_Reg(message_s* ptrAux)
{
    return _Generic_Read_Reg(ptrAux);
}
int _Set_Bool_Reg(message_s* ptrAux)
{
    return _Generic_Set_Reg(ptrAux);
}
int _Set_Single_Reg(message_s* ptrAux)
{
    return _Generic_Set_Reg(ptrAux);
}
int _Read_Exception(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamnho 4 onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX -> Estatus de exceção
    //  0xXX 0xXX -> CRC
    int sizeReply = 4;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    //unsigned char exception = ptrReply[1];
    //Implementar
    //Trata exceção
    return _replyOK;
}
int _Diagnostics(message_s* ptrAux)
{
    return _Generic_Set_Reg(ptrAux);
}
int _Get_Event_Counter(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 5 onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX 0xXX -> Valor do contador de eventos
    //  0xXX 0xXX -> CRC
    int sizeReply = 5;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    int eventCounter;
    memcpy(&eventCounter, &ptrReply[1], sizeof(short));
    return _replyOK;
}
int _Get_Event_Log(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 5 onde é composto por
    //  0x0C -> Codigo da função;
    //  0x10 -> Numero de eventos;
    //  0xXX 0xXX -> Primeiro evento registrado;
    //  ...
    //  0xXX 0xXX -> n eventos
    //  0xXX 0xXX -> CRC
    unsigned char ptrReply[0xFF];
    if(i2cRead_master(ptrAux->message.addr, ptrReply) != i2cStat_OK)
        return _replyFail;
    int sizeReply = (ptrReply[1]*sizeof(short)) + 4;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    int eventCounter;
    memcpy(&eventCounter, &ptrReply[1], sizeof(short));
    return _replyOK;
}
int _Set_Mult_Bool_Reg(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 5 onde é composto por
    //  0x0F -> Codigo da função;
    //  0xXX 0xXX -> Endereço do primeiro registrador;
    //  0xXX 0xXX -> Quantidade de registradores escritos;
    //  0xXX 0xXX -> CRC
    //  Da para utilizar o set generic
    return _Generic_Set_Reg(ptrAux);
}
int _Set_Mult_Num_Reg(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamnho 7 onde é composto por
    //  0x10 -> Codigo da função;
    //  0xXX 0xXX -> Endereço do primeiro registrador
    //  0xXX 0xXX -> Quantidade de registradores escritos;
    //  0xXX 0xXX -> CRC
    return _Generic_Set_Reg(ptrAux);
    // int sizeReply = 7;
    // unsigned char ptrReply[sizeReply];
    // if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
    //     return _replyFail;
    // if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
    //     return _replyFail;
    // sizeReply -= 2;
    // for (unsigned i = 0; i < sizeReply; i++)
    //     if (ptrReply[i] != ptrAux->message.ptrMessage[i])
    //         return _replyFail;
    // return _replyOK;
}
int _Report_Slave_ID(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4 + nBytes onde é composto por
    //  0x11 -> Codigo da função;
    //  0x10 -> Comprimento dos dados de identificação;
    //  0xXX ... 0xXX -> Dados de identificação;
    //  0xXX 0xXX -> CRC
    int len = 0;
    switch (ptrAux->message.addr)
    {
    case SysI2C_ADDR_WiFi:
        len = 15;
        break;
    case SysI2C_ADDR_Rec_Display:
        return _replyFail;
        break;
    default:
        return _replyFail;
        break;
    }
    unsigned char ptrReply[len];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, len) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, len))//verifica se está ok
        return _replyFail;

    int *ptrTimestamp_local = (int *)reg_ptr_int(0);
    int timestamp_ref;
    switch (ptrAux->message.addr)
    {
    case SysI2C_ADDR_WiFi:
        memcpy(&timestamp_ref, &ptrReply[9], sizeof(int));
        if ((*ptrTimestamp_local) < timestamp_ref)
            *ptrTimestamp_local = timestamp_ref;
        return _replyOK;
        break;
    case SysI2C_ADDR_Rec_Display:
        return _replyFail;
        break;
    default:
        return _replyFail;
        break;
    }
    //Processa identificação
    return _replyOK;
}
int _Read_File_Record(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4 + nBytes onde é composto por
    //  0x14 -> Codigo da função;
    //  0xXX -> Comprimento dos dados de identificação;
    //  0xXX ... 0xXX -> Dados de identificação;
    //  0xXX 0xXX -> CRC
    unsigned char ptrReply[0xFF];
    if(i2cRead_master(ptrAux->message.addr, ptrReply) != i2cStat_OK)
        return _replyFail;
    int sizeReply = ptrReply[1];
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    //Processa dados
    return _replyOK;
}
int _Write_File_Record(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4 + nBytes onde é composto por
    //  0x15 -> Codigo da função;
    //  0xXX -> Quantidade de subrequisições;
    //  0xXX 0xXX -> Referencia do arquivo;
    //  0xXX 0xXX -> Endereço inicial dentro do arquivo;
    //  0xXX 0xXX -> Quantidade de Registros;
    //  0xXX 0xXX -> CRC
    int sizeReply = 10;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    sizeReply-=2;//remove o CRC
    for (int i = 0; i < sizeReply; i++)
        if (ptrAux->message.ptrMessage[i] != ptrReply[i])
            return _replyFail;
    return _replyOK;
}
int _Mask_Write_Register(message_s* ptrAux)
{
    return _replyFail;
}
int _RW_Mult_Registers(message_s* ptrAux)
{
    //A resposta dessa mensagem tem padrão de tamanho 4 + nBytes onde é composto por
    //  0x17 -> Codigo da função;
    //  0xXX -> Quantidade de bytes recebidos;
    //  0xXX ... 0xXX -> Dados
    //  0xXX 0xXX -> CRC
    short nBytes = Sys_RegMap_GetRegBytes(ptrAux->message.RegAddr);
    if (nBytes>0)
        nBytes *= ptrAux->message.nReg;
        else
            return _replyFail;
    //Não está implementado a leitura de registradores booleanos
    // else if (nBytes == 0)
    //     {
    //         nBytes = (ptrAux->message.nReg%8) ? 1 : 0;
    //         nBytes += ptrAux->message.nReg/8;
    //     }
    int sizeReply = 4 + nBytes;
    unsigned char ptrReply[sizeReply];
    if(i2cReadLenght_master(ptrAux->message.addr, ptrReply, sizeReply) != i2cStat_OK)
        return _replyFail;
    if(!CHECK_CRC16(ptrReply, sizeReply))//verifica se está ok
        return _replyFail;
    reg_write_vet(&ptrReply[2], ptrAux->message.nReg, ptrAux->message.RegAddr);
    return _replyOK;
}
int _Read_FIFO_Queue(message_s* ptrAux)
{
    return _replyFail;
}
int _Read_Device_ID(message_s* ptrAux)
{
    return _replyFail;
}

void _message_loop_aux()
{
    int statusAux = 0;
    
    // for (int i = 0; i < 3; i++)//Tenta enviar a mensagem até 3x 
    // {
    //Verifica se esta esperando uma resposta
    task_scheduler_pause();
    statusAux = i2cSend_master_modbus(&_ptrQueue->message);
    task_scheduler_continue();
        // if (statusAux == i2cStat_OK)
        //     break;
    //     task_delay_ms(Sys_Modbus_Time_Reply);
    // }
    task_delay_ms(Sys_Modbus_Time_Reply);
    //Do inicio do envio da mensagem até a finalização e inicio recepção espera até 10ms
    task_scheduler_pause();
    switch (_ptrQueue->message.code)
    {
    case Modbus_Get_Bool_Reg:
        statusAux = _Get_Bool_Reg(_ptrQueue);
        break;
    case Modbus_Get_Discrete_Input:
        statusAux = _Get_Discrete_Input(_ptrQueue);
        break;
    case Modbus_Get_Num_Reg:
        statusAux = _Get_Num_Reg(_ptrQueue);
        break; 
    case Modbus_Read_Input_Reg:
        statusAux = _Read_Input_Reg(_ptrQueue);
        break;
    case Modbus_Set_Bool_Reg:
        statusAux = _Set_Bool_Reg(_ptrQueue);
        break;
    case Modbus_Set_Single_Reg:
        statusAux = _Set_Single_Reg(_ptrQueue);
        break;
    case Modbus_Read_Exception:
        statusAux = _Read_Exception(_ptrQueue);
        break;
    case Modbus_Diagnostics:
        statusAux = _Diagnostics (_ptrQueue);
        break;
    case Modbus_Get_Event_Counter:
        statusAux = _Get_Event_Counter(_ptrQueue);
        break;
    case Modbus_Get_Event_Log:
        statusAux = _Get_Event_Log(_ptrQueue);
        break;
    case Modbus_Set_Mult_Bool_Reg:
        statusAux = _Set_Mult_Bool_Reg(_ptrQueue);
        break;
    case Modbus_Set_Mult_Num_Reg:
        statusAux = _Set_Mult_Num_Reg(_ptrQueue);
        break;
    case Modbus_Report_Slave_ID:
        statusAux = _Report_Slave_ID(_ptrQueue);
        break;
    case Modbus_Read_File_Record:
        statusAux = _Read_File_Record(_ptrQueue);
        break;
    case Modbus_Write_File_Record:
        statusAux = _Write_File_Record(_ptrQueue);
        break;
    case Modbus_Mask_Write_Register:
        statusAux = _Mask_Write_Register(_ptrQueue);
        break;
    case Modbus_RW_Mult_Registers:
        statusAux = _RW_Mult_Registers(_ptrQueue);
        break;
    case Modbus_Read_FIFO_Queue:
        statusAux = _Read_FIFO_Queue(_ptrQueue);
        break;
    case Modbus_Read_Device_ID ... 0xFF:
        statusAux = _Read_Device_ID(_ptrQueue);
        break;
    default:
        break;
    }
    if (statusAux == _replyFail)
        i2cReadClearBuffer_master(_ptrQueue->message.addr);
    task_scheduler_continue();
}
void message_loop()
{
    if (!_message_init)
        return;
    if (_message_queue_lenght == 0)
        return;
    if (_ptrQueue->message.addr == SysI2C_ADDR_ALL)
    {
        for (int i = 0; i < sizeof(SysI2C_ADDR_Slave_Vet); i++)
        {
            _ptrQueue->message.addr = SysI2C_ADDR_Slave_Vet[i];
            _message_loop_aux();
            task_delay_ms(Sys_Modbus_Time_Reply);
        }
    }else
        _message_loop_aux();
    // Vai para a proxima mensagem
    _ptrQueue->message = _message_default;
    _ptrQueue = (message_s*)_ptrQueue->ptrNext;
    if (_message_queue_lenght > 0)
        _message_queue_lenght--;
    
    task_delay_ms(Sys_Modbus_Time_Reply);
}

