#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H
#include "board_defs.h"
#include "i2c.h"

enum MessageStatus
{
    Message_QueueOverload=-1,
    Message_Fail,
    Message_Ok
};//

void message_Init();
void message_Delete();
int message_QueueLen();
int message_QueueSize();
int message_New(i2c_modbus_s newMessage);

void message_loop();
#endif