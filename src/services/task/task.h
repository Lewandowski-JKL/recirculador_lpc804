#ifndef TASK_H
#define TASK_H

typedef void task_func_t(void *arg);

void task_schedulerInit();
void task_new(task_func_t *ptrFunc, char *taskName, unsigned int frequency, unsigned char priority, void *arg);
void task_delay_ms(unsigned long time_ms);
void task_delay_ticks(unsigned long ticks);

// void beginTasks(unsigned int SysFreq);
// int newTask(task_func_t *ptrFunc, char *name, unsigned int frequency, unsigned char priority, void *ptrData);
// void scheduler(void *arg);


#endif