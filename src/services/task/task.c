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

//Estrutura da task
typedef struct task_s
{
    task_func_t *ptrFunc;
    unsigned long lastExecution;
    unsigned char priority;
    void *ptrData;
    char TaskName[Sys_task_name_length];
}myTask;

void task_schedulerInit()
{

    scheduler_on = 1;
}

void task_new(task_func_t *ptrFunc, char *taskName, unsigned int frequency, unsigned char priority, void *arg)
{

}
void task_delay_ms(unsigned long time_ms)
{
    unsigned long time_end = SysTickGetTime_ms()+time_ms; 
    while (SysTickGetTime_ms() <= time_end);
}
void task_delay_ticks(unsigned long ticks)
{
    unsigned long ticks_end = SysTickGetTicks()+ticks; 
    while (SysTickGetTicks() <= ticks_end);
}