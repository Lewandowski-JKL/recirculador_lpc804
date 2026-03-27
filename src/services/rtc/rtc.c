#include "rtc.h"
#include <stdio.h>
#include <stdlib.h>
#include "board_defs.h"
#include "registers_manager.h"
#include "interrupt.h"
#include "i2c.h"

bool _flagBegin = false;
unsigned long _last_time_resynchronization = 0;
unsigned long _time_resynchronization = 3600000;//A cada hora
unsigned char _rx[8] = {0};
unsigned char _tx[8] = {0};
void RTC_begin(void *arg)
{
    _flagBegin = true;
}
void RTC_resynchronization()
{
    unsigned char reg = 0;
    i2cSend_master(SysI2C_ADDR_RTC_BQ32000, _rx, 1);
    i2cReadLenght_master(SysI2C_ADDR_RTC_BQ32000, &_rx[1], sizeof(_rx));
}

void RTC_setValue()
{
    unsigned char reg = 0;
    i2cSend_master(SysI2C_ADDR_RTC_BQ32000, &reg, 1);
    i2cSend_master(SysI2C_ADDR_RTC_BQ32000, _tx, sizeof(_tx));
}
void RTC_loop(void *arg)
{
    if (!_flagBegin)
        return;
    unsigned long long dt = SysTickGetTime_ms() - _last_time_resynchronization;
    if (dt > _time_resynchronization)
        RTC_resynchronization();
}
void RTC_set_resynchronization_time(unsigned long time)
{
    _time_resynchronization = time;
}