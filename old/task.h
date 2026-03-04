#ifndef TASK_H
#define TASK_H

typedef void task_func_t(void *arg);

void beginTasks(unsigned int SysFreq);

int newTask(task_func_t *ptrFunc, char *name, unsigned int frequency, unsigned char priority, void *ptrData);

void scheduler(void *arg);


#endif