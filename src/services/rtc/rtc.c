#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board_defs.h"
#include "registers_manager.h"
#include "interrupt.h"
#include "i2c.h"
#include "rtc.h"

#define secMin              (60) //segundos por minuto
#define secHr               (3600) //segundos por hora
#define secDay              (86400) //segundos por dia
#define secWeek             (604800) //segundos por semana
#define secMonth_average    (2629743) //segundos por mes
#define secYear_average     (31556926) //segundos por ano
#define secYear             (31536000) //segundos por ano
#define daysYear            ((short)365) //dias por ano
#define daysYearB           ((short)366) //dias por ano bissexto

//#define scheduleReturnMode(v)        ((v) & 0b00000000000000000000000000000111)
#define scheduleReturnWeekday(v)     (((v) & 0b11111110000000000000000000000000) >> 25)
#define scheduleReturnTimeStart(v)   (((v) & 0b00000001111111111100000000000000) >> 14)
#define scheduleReturnTimeEnd(v)     (((v) & 0b00000000000000000011111111111000) >> 3)


//29
unsigned char _dayToMouth[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; //dias de cada mes

bool _flagBegin = false;
unsigned long _last_time_resynchronization = 0;
unsigned long _time_resynchronization = 3600000;//A cada hora
unsigned int _timestamp = 0;
unsigned int *_ptrTimestamp;
typedef struct __attribute__((packed)) RTC_data_s
{
    unsigned char seconds;
    unsigned char minutes;
    unsigned char cent_hours;
    unsigned char day;
    unsigned char date;
    unsigned char month;
    unsigned char years;
    unsigned char cal_cfg1;
    unsigned char tch_2;
    unsigned char cfg_2;
    ///////////////////
    unsigned char sf_key_1;
    unsigned char sf_key_2;
    unsigned char sfr;
}RTC_data_s;


unsigned long long _RTC_convertTimeToTimestamp(RTC_data_s time);
RTC_data_s _RTC_convertTimestampToTime(unsigned long long timestamp);

void RTC_begin(unsigned int *ptrTimestamp)
{
    _flagBegin = true;
    if (ptrTimestamp == NULL)
        _ptrTimestamp = &_timestamp;
        else
            _ptrTimestamp = ptrTimestamp;
    //(*ptrTimestamp) = 1775055600;
    //RTC_setValue((*ptrTimestamp));
    RTC_resynchronization();
}

unsigned char _RTC_convertToReg_aux(int value)
{
    return ((value/10)<<4) + (value%10);
}
unsigned short _RTC_convertToTime_aux(char value)
{
    return (10*((0b1110000 & value) >> 4)) + (value&0b1111);
}
RTC_data_s _RTC_convertTimestampToTime(unsigned long long timestamp)
{
    RTC_data_s timeAux;
    if (timestamp < Sys_timestamp_min)//Define o valor minimo de timestamp que é no inicio de 2024
        timestamp = Sys_timestamp_min;
    //Faz a diferença do timestamp atual e guardaa diferença em dias
    timeAux.seconds = timestamp  % secMin;
    timeAux.minutes = (timestamp % secHr)  / secMin;
    timeAux.cent_hours = ((timestamp % secDay) / secHr);
    timestamp-=Sys_timestamp_min;
    unsigned int daysLeft = timestamp/secDay;
    timeAux.years = 0;
    while (true)
    {
        if (timeAux.years > 3000) //Se o ano é maior que 3000 para de executar o loop
            break;
        if (timeAux.years%4)//Verifica se é ano bissexto, no caso de resto zero qer dizer que o ano é bissexto
        {
            if (!(daysLeft/daysYear))//Verifica se tem mais dias do que um 365 dias sobrando
                break;
            daysLeft -= daysYear; //Remove os 365 dias do valor total
        }else
            {
                if (!(daysLeft/daysYearB))//Verifica se tem mais que 366 dias sobrando
                    break;
                daysLeft -= daysYearB;//Remove 366 dias do valor total
            }
        timeAux.years++;//aumenta um ano
    }
    timeAux.month = 0;
    while (true)
    {
        if (timeAux.month > 10)
            break;
        if (timeAux.years%4)//Verifica se não é ano bissexto
        {
            if (!(daysLeft/_dayToMouth[timeAux.month]))//Verifica se tem mais dias que completa o mes
                break;
                else
                    daysLeft -=_dayToMouth[timeAux.month];//Remove o mês das datas totais
        }else//é Ano bissexto
            {
                if (timeAux.month==1)//verifica se é fevereiro
                {
                    if (!(daysLeft/29))//verifica se tem menos do que um mês restante
                    break;
                    else
                        daysLeft -= 29; //remove 29 dias 
                }else
                    {
                        if (!(daysLeft/_dayToMouth[timeAux.month]))//Verifica se tem mais dias que completa o mes
                        break;
                        else
                            daysLeft -=_dayToMouth[timeAux.month];//Remove o mês das datas totais
                    }
            }
        timeAux.month++;//Adiciona um mes
    }
    timeAux.month++;
    timeAux.date =  (timestamp % secDay) ? (daysLeft+1) : daysLeft;
    if (!(timeAux.cent_hours | timeAux.minutes | timeAux.seconds))
        timeAux.date++;
    int weekDay = (timestamp % secWeek);
    weekDay = (weekDay % secDay) ? ((weekDay/secDay) + 2) : ((weekDay/secDay) + 1);

    //RTC_data_s vAux = RTC_var;
    timeAux.seconds = _RTC_convertToReg_aux(timeAux.seconds);
    timeAux.minutes = _RTC_convertToReg_aux(timeAux.minutes);
    timeAux.cent_hours = _RTC_convertToReg_aux(timeAux.cent_hours);
    timeAux.day = _RTC_convertToReg_aux(weekDay);
    timeAux.date = _RTC_convertToReg_aux(timeAux.date);
    timeAux.month = _RTC_convertToReg_aux(timeAux.month);
    timeAux.years = _RTC_convertToReg_aux(timeAux.years);
    return timeAux;
}

unsigned long long _RTC_convertTimeToTimestamp(RTC_data_s time)
{
    unsigned char sec = _RTC_convertToTime_aux(time.seconds);    //variavel que guarda os segundos
    unsigned char min = _RTC_convertToTime_aux(time.minutes);      //variavel que guarda os minutos
    unsigned char hr = _RTC_convertToTime_aux(time.cent_hours);       //variavel que guarda as horas       
    unsigned char day = _RTC_convertToTime_aux(time.date) - 1;      //variavel que guarda o dia
    unsigned char month = _RTC_convertToTime_aux(time.month)-1;
    unsigned short year = _RTC_convertToTime_aux(time.years);
    int secMonthAux = 0;
    int timestampAux = 0;
    for (int i = 0; i < month; i++)
    {
        if (year%4)//Verifica se é ano bissexto, no caso de resto zero qer dizer que o ano é bissexto
        {
            secMonthAux += _dayToMouth[i] * secDay;
        }else
            {
                if (i==1)
                    secMonthAux += 29 * secDay;
                    else
                        secMonthAux += _dayToMouth[i] * secDay;
            }
    }
    int bissexto = (year/4)*secDay;
    bissexto += (year%4) ? secDay : 0;
    timestampAux = Sys_timestamp_min;
    timestampAux += (year*secYear) + secMonthAux + (day*secDay) + (hr*secHr) + (min*secMin) + sec + bissexto;
    return timestampAux;
}

void RTC_resynchronization()
{
    RTC_data_s timeAux = _RTC_convertTimestampToTime((*_ptrTimestamp));
    unsigned char reg = 0;
    i2cSend_master(SysI2C_ADDR_RTC_BQ32000, &reg, 1);
    i2cReadLenght_master(SysI2C_ADDR_RTC_BQ32000, (unsigned char*)&timeAux, 10);
    unsigned long long timestampAux = _RTC_convertTimeToTimestamp(timeAux);
    if ((timeAux.minutes & 0b10000000) || (timestampAux < Sys_timestamp_min) || (timestampAux > 7268702400))//Detecta uma falha
    {
        (*_ptrTimestamp) = Sys_timestamp_min;
        timeAux = _RTC_convertTimestampToTime(Sys_timestamp_min);
        RTC_setValue(Sys_timestamp_min);
    }
    (*_ptrTimestamp) = _RTC_convertTimeToTimestamp(timeAux);
}

void RTC_setValue(unsigned int timestampRef)
{
    unsigned char reg[11] = {0};
    RTC_data_s timeAux = _RTC_convertTimestampToTime(timestampRef);
    timeAux.seconds &= 0b01111111;
    timeAux.minutes &= 0b01111111;
    timeAux.day     &= 0b00000111;
    timeAux.date    &= 0b00111111;
    timeAux.month   &= 0b00011111;
    timeAux.cal_cfg1 = 0x00;
    timeAux.tch_2 = 0x00;
    timeAux.cfg_2 = 0x00;
    memcpy(&reg[1], &timeAux, 10);
    //Escreve os dados do horario atualizados
    i2cSend_master(SysI2C_ADDR_RTC_BQ32000, reg, 11);
}

void RTC_loop(void *arg)
{
    if (!_flagBegin)
       return;
    unsigned long long dt = SysTickGetTime_ms() - _last_time_resynchronization;
    if (dt < _time_resynchronization)
        return;
    RTC_resynchronization();
    _last_time_resynchronization =  SysTickGetTime_ms();
}

void RTC_set_resynchronization_time(unsigned long time)
{
    _time_resynchronization = time;
}