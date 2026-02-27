/**
 * @file myCalendar.c
 * @author Jackson Kenedi Lewandowski (jackson.lewandowski@komlog.com/jacksonlewandowski.work@komlog.com)
 * @brief Essa biblioteca faz a gestão de um calendário interno e de agendamentos
 * @version 0.1
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "calendar.h"
#include "app_config.h"
#include <time.h>
#include "registers_manager.h"

#define secMin 60 //segundos por minuto
#define secHr 3600 //segundos por hora
#define secDay 86400 //segundos por dia
#define daysYear ((short)365) //dias por ano
#define daysYearB ((short)366) //dias por ano bissexto
//29
unsigned char dayToMouth[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; //dias de cada mes

//Header de função
void _attMyTime();

//Ponteiro para auxiliar no controle do timestamp
volatile int *ptrTimestamp = 10;//&IntRegisters[regMapTimestamp-regMapIntOffset-1];
unsigned char seg;      //variavel que guarda os segundos
unsigned char min;      //variavel que guarda os minutos
unsigned char hr;       //variavel que guarda as horas       
unsigned char day;      //variavel que guarda o dia
unsigned char month;    //variavel que guarda o mes
unsigned short year;    //variavel que guarda o ano
/**
 * @brief Faz a verificação se tiver um agendamento para a data e horario atual
 * 
 * @return true 
 * @return false 
 */
bool schedulingTest()
{
    unsigned char mask = (0b01 << (7-getWeekDay()));//Carrega o valor da mascara semanal
    unsigned short timeNow = min + getHour()*60;//Carrega o tempo atual em minutos
    
    unsigned int shedule = 0;//Agendamento
    /*
        O agendamento fica guardado em um registrador de 32 bits e é distribuido como visto abaixo
        0b0000 0000 0000 0000 0000 0000 0000 0000
         |  semana | Tempo Inicial| Tempo Final  |
        - Semana: Os primeiros 8 bits mais significativos é a mascara referente ao dia da semana,
                sendo distribuido dessa forma -> | x | seg | ter | qua | qui | sex | sab | dom |
        - Tempo Inicial: Se refere ao tempo inicial em que o agendamento passara a funcionar. O tempo
                é dado em minutos.
        -Tempo Final: Se refere ao tempo em que o agendamento vai parar de funcionar. O tempo é dado
                em minutos.
     */
    unsigned short addrAux = 0;//Endereço auxiliar
    for (unsigned char i = 0; i < 10; i++)//Percorre todos os agendamento -> até 10
    {
        addrAux = 10;//regMapAgendamentosInit+i;//Endereço do registrador que está o agendamento
        shedule = getIntWithAddr(addrAux);//Recolheo valor presente no registrador
        if ((shedule & FlagSheduleOn) == FlagSheduleOn) //Verifica se é um agendamento ativo
        {
            //Verifica se tem um agendamento na data atual da semana
            if ((shedule >> 24)&mask)
            {
                unsigned short timeInit = ((shedule >> 12) & 0x7FF); //Carrega o tempo em que o agendamento tem inicio
                unsigned short timeEnd = (shedule & 0x7FF); //Carrega o tempo em que o agendamento finaliza
                if ((timeNow>timeInit)&&(timeNow<timeEnd)) //Se o tempo atual estiver entre os dois retorna true
                   return true;
            }
        }
    }
    return false; //Não tem agendamento para esse momento
}
/**
 * @brief Retorna o valor do segundo atual
 * 
 * @return unsigned char 
 */
unsigned char getSecond()
{
    seg = *ptrTimestamp%secMin;
    return seg;
}
/**
 * @brief Retorna o valor do minuto atual
 * 
 * @return unsigned char 
 */
unsigned char getMin()
{
    min = (*ptrTimestamp/secMin)%secMin;
    return min;
}
/**
 * @brief Retorna o valor da hora atual
 * 
 * @return unsigned char 
 */
unsigned char getHour()
{
    hr = (*ptrTimestamp/secHr)%24;
    return hr;
}
/**
 * @brief Retorna o dia atual
 * 
 * @return unsigned char 
 */
unsigned char getDay()
{
    _attMyTime();
    day++;
    return day;
}
/**
 * @brief Retorna o mes atual
 * 
 * @return unsigned char 
 */
unsigned char getMount()
{
    _attMyTime();
    month++;
    return month;
}
/**
 * @brief Retorna o ano atual
 * 
 * @return unsigned short 
 */
unsigned short getYear()
{
    _attMyTime();
    return year;
}
/**
 * @brief Retorna o dia da semana distribuido dessa forma -> | x | seg | ter | qua | qui | sex | sab | dom |
 * 
 * @return unsigned char 
 */
unsigned char getWeekDay()
{
    unsigned int daysLeft = (*ptrTimestamp-MinTimestamp)/secDay;//2024 iniciou na segunda
    daysLeft = (daysLeft + 1) % 7;
    return (char)daysLeft;
}
/**
 * @brief Atualiza a data atual 
 * 
 */
void _attMyTime()
{
    if (*ptrTimestamp < MinTimestamp)//Define o valor minimo de timestamp que é no inicio de 2024
        *ptrTimestamp = MinTimestamp;
    year = 2024; //ano minimo
    //Faz a diferença do timestamp atual e guardaa diferença em dias
    unsigned int daysLeft = (*ptrTimestamp-MinTimestamp)/secDay;
    while (true)
    {
        if (year > 3000) //Se o ano é maior que 3000 para de executar o loop
            break;
        if (!(year%4))//Verifica se é ano bissexto, no caso de resto zero qer dizer que o ano é bissexto
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
        year++;//aumenta um ano
    }
    month = 0;
    while (true)
    {
        if (month > 10)
            break;
        if ((!(year%4))&&(month==1))//Verifica se é ano bissexto e se é fevereiro
        {//aqui era (year%4) -> verificar
            if (!(daysLeft/29))//verifica se tem menos do que um mês restante
                break;
                else
                    daysLeft -= 29; //remove 29 dias 
        }else
            if (!(daysLeft/dayToMouth[month]))//Verifica se tem mais dias que completa o mes
                break;
                else
                    daysLeft -=dayToMouth[month];//Remove o mês das datas totais
        month++;//Adiciona um mes
    }
    day = daysLeft; //Guarda o restante dos dias
}
/**
 * @brief Escreve um valor no timestamp
 * 
 * @param setTimestamp 
 */
void setTimestamp(unsigned int setTimestamp)
{
    if (setTimestamp < MinTimestamp)
    {
        *ptrTimestamp=MinTimestamp;
        return;
    }
    *ptrTimestamp = setTimestamp;
}
/**
 * @brief Atualiza o valor do timestamp
 * 
 */
void tickTimestamp()
{
    unsigned int value = *ptrTimestamp+1;
    setTimestamp(value);
}
