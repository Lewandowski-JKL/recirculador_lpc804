/**
 * @file scheduling.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-04-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduling.h"
#include "board_defs.h"
#include "registers_manager.h"
/*
        Nesse caso seguir uma estrutura parecida mas com modos de funcionamento integrados, mantém a granulometria de 1 minuto:
        reg uInt_32 -> 0b0000 0000 0000 0000 0000 0000 0000 0000
        0b | 000 0000 | 0000 0000 000 | 0 0000 0000 00 | 000              | 
           | DST QQSS | 0->2048       | 0->2048        | 0 -> 7           |
           | Dia Sem. | Tempo Inicial | Tempo Final    | Modo de Operação |

        Se os dias da semana estiver todos em zero e ele é considerado desativado

        Modos de atuação:

        Para o recirculador é ignorado o modo de atuação portanto não importa

        0b000 -> TURN ON MODE: Apenas um comando para ligar;
        0b001 -> TURN OFF MODE: Apenas um comando para desligar;
        0b010 -> TURN ON/OFF MODE: Liga no tempo inicial e desliga no tempo final;
        0b011 -> TURN OFF/ON MODE: Desliga no tempo inicial e liga no tempo final;
        0b100 -> ON MODE: Liga e permanece ligado entre o tempo inicial e final
        0b101 -> OFF MODE: Desliga e permanece desligado entre o tempo inicial e final
        0b110 -> TIMER ON MODE: Fica ligado por X segundos e desliga;
        0b111 -> TIMER OFF MODE: Figa desligado por X segundos e então liga;
*/

/***************************************************************
 *  Scheduling Module - Versão Final Corrigida e Estável
 *  ------------------------------------------------------------
 *  Compatível com o formato de 32 bits do schedule:
 *  [31..25] Dias da semana (bitmask)
 *  [24..14] Horário inicial (minutos do dia)
 *  [13..3 ] Horário final   (minutos do dia)
 *  [2..0  ] Modo (0–7)
 ***************************************************************/

#define scheduleReturnMode(v)        ((v) & 0b00000000000000000000000000000111)
#define scheduleReturnDay(v)         (((v) & 0b11111110000000000000000000000000) >> 25)
#define scheduleReturnTimeStart(v)   (((v) & 0b00000001111111111100000000000000) >> 14)
#define scheduleReturnTimeEnd(v)     (((v) & 0b00000000000000000011111111111000) >> 3)

#define secMin              (60) //segundos por minuto
#define secHr               (3600) //segundos por hora
#define secDay              (86400) //segundos por dia
#define secWeek             (604800) //segundos por semana
#define secMonth_average    (2629743) //segundos por mes
#define secYear_average     (31556926) //segundos por ano
#define secYear             (31536000) //segundos por ano
#define daysYear            ((short)365) //dias por ano
#define daysYearB           ((short)366) //dias por ano bissexto


/* ============================================================
 *  Cálculo do dia e horário atual
 * ============================================================ */
unsigned char weekDay(unsigned int timestamp)
{
    timestamp -= Sys_timestamp_min;
    unsigned int dayAux = (timestamp / secDay);
    dayAux = (timestamp % secDay) ? dayAux + 1 : dayAux;
    dayAux %= 7;
    return (0b1000000 >> dayAux); // segunda = bit 6, domingo = bit 0
}
short minute(unsigned int timestamp)
{
    return ((timestamp % secDay)/secMin);
}
short second(unsigned int timestamp)
{
    return (timestamp % secMin);
}


/* ============================================================
 *  Função principal
 * ============================================================ */
bool schedulingTest(unsigned int timestamp)
{
    short timeStart = 0;
    short timeEnd = 0;
    short time = 0;
    unsigned int *schedule = (unsigned int*) reg_ptr_int(Sys_RegMap_Schedules_0);
    for (unsigned char i = 0; i < Sys_schedulers_total; i++)
    {
        //Agendamento valido para o horario
        if (scheduleReturnDay(schedule[i]) & weekDay(timestamp))
        {
            timeStart = scheduleReturnTimeStart(schedule[i]);
            timeEnd = scheduleReturnTimeEnd(schedule[i]);
            time = minute(timestamp);
            if (timeEnd < timeStart)
            {
                if (!((time >= timeEnd) && (time < timeStart)))
                    return true;
            }else
            {
                if ((time < timeEnd) && (time >= timeStart))
                    return true;
            }
        }
    }
    return false;
}
