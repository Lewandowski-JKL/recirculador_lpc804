/**
 * @file task.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef TASK_H
#define TASK_H
/**
 * @brief Modelo de função para o sistema de Task.
 * O sistema de task é utilizado pois não é possível rodar o RTOS no LPC804 pois ele tem pouca memória.
 * 
 * Para criar uma task deve seguir o seguinte padrão
 * 
 ***************************************************
 ***************************************************
    -> Criar uma struct com as varíaveis que devem permanecer salvas entre os processos da task
    typedef struct task_s
    {
        int i;
        int x;
        ...
    }task_s;
    -> Deve declarar as variaveis
    task_s task_var = 
    {
        .i = 0,
        .x = 0,
        ...
    };
    ->deve criar a função que executa alguma ação, no caso apenas a parte do "loop" da task
    void task_loop(void *arg)
    {
        task_s *_var = (task_s*)arg;
        _var->x = _var->i;
        ...
    }
 ***************************************************
    ->Para criar a task deve chamar a função dessa forma:
    task_new(task_loop, "task", 2, 0, (void*)&task_var);
 ***************************************************
 ***************************************************
 * @param arg 
 * @return 
 */
typedef void task_func_t(void *arg);
void task_schedulerInit();
void task_new(task_func_t *ptrFunc, char *taskName, unsigned int frequency, unsigned char priority, void *arg);
void task_delay_ms(unsigned long time_ms);
void task_delay_ms_until(unsigned int *ptr, unsigned long time_ms);
void task_delay_ticks(unsigned long ticks);
void task_delay_ticks_until(unsigned int *ptr, unsigned long ticks);

#endif