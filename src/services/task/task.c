#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "interrupt.h"
#include "board_defs.h"

#ifndef Sys_task_max
    #define Sys_task_max            10
#endif

#ifndef Sys_task_name_length
    #define Sys_task_name_length            20
#endif

int scheduler_on = 0;
int tasks_on=0;
int pause = 0;

//Estrutura da task
typedef struct task_s
{
    task_func_t *ptrFunc;
    unsigned long long lastExecution;
    unsigned long dt_us;
    unsigned char priority;
    void *ptrData;
    char TaskName[Sys_task_name_length];
}task_s;

const task_s task_s_default = 
{
    .ptrFunc = NULL,
    .lastExecution = 0,
    .priority=0,
    .ptrData=NULL
};

task_s task_vet[Sys_task_max];

void isrScheduler(volatile void *arg)
{
    if (pause)
        return;
    unsigned long long time_us_aux = 0;
    unsigned long long time_dt = 0;
    for (int i = 0; i < Sys_task_max; i++)
    {
        if (task_vet[i].ptrFunc != NULL)
        {
            time_us_aux = SysTickGetTime_us();
            time_dt = time_us_aux - task_vet[i].lastExecution;
            if (time_dt >= task_vet[i].dt_us)
            {
                task_vet[i].lastExecution = time_us_aux;
                task_vet[i].ptrFunc(task_vet[i].ptrData);
                return;
            }
        }
    }
    
}

void task_schedulerInit()
{
    if (scheduler_on)
        return;
    scheduler_on = 1;
    SysTickBeginISR(SysTickFrequency, isrScheduler);
}

void task_vet_init()
{
    for (int i = 0; i < Sys_task_max; i++)
        task_vet[i]=task_s_default;
    tasks_on = 1;
}

void task_new(task_func_t *ptrFunc, char *taskName, unsigned int frequency, unsigned char priority, void *arg)
{
    //testa se o vetor estatico ja foi iniciado, se não inicia todos
    if (tasks_on == 0)
        task_vet_init();
    //verifica se estourou o numero de tasks
    /*if ((tasks_on >> 1u) == Sys_task_max)
        return;*/

    task_s *ptrAux;
    for (int i = 0; i < Sys_task_max; i++)
        if (task_vet[i].ptrFunc == NULL)
        {
            ptrAux = &task_vet[i];
            break;
        }

    ptrAux->ptrFunc = ptrFunc;
    ptrAux->dt_us = 1000000/frequency;
    ptrAux->priority = priority;
    ptrAux->ptrData = arg;

    for (int i = 0; i < Sys_task_name_length; i++)
    {
        ptrAux->TaskName[i] = taskName[i];
        if (taskName[i] == '\0')
            break;
    }
    
    tasks_on += 0b10;

    if (scheduler_on == 0)
        task_schedulerInit();
    
}

void task_delay_ms(unsigned long time_ms)
{
    unsigned long long time_end = SysTickGetTime_ms()+time_ms; 
    while (SysTickGetTime_ms() < time_end);
}
void task_delay_ms_until(unsigned long long *ptr, unsigned long time_ms)
{
    unsigned long long time_end = *ptr+time_ms; 
    *ptr = time_end;
    while (SysTickGetTime_ms() < time_end);

}
void task_delay_ticks(unsigned long ticks)
{
    unsigned long ticks_end = SysTickGetTicks()+ticks; 
    while (SysTickGetTicks() <= ticks_end);
}
void task_delay_ticks_until(unsigned long long *ptr, unsigned long ticks)
{
    unsigned long long time_end = *ptr+ticks; 
    *ptr = time_end;
    while (SysTickGetTime_ms() < time_end);
}
void task_scheduler_pause()
{
    pause = 1;
}
void task_scheduler_continue()
{
    pause = 0;
}