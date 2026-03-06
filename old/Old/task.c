#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "interrupt.h"


//Variaveis globais
//Salva a frequencia dos ticks do SO
unsigned int SysFrequency = 0;

//Numerode Tasks criadas
int taskNum = 0;

//Flag que sinaliza que a task foi iniciada
unsigned char taskIniciada = 0;

//Estrutura da task
typedef struct myTask
{
    taskFuncType *ptrFunc;
    int delay;
    unsigned char priority;
    unsigned long lastExecution;
    void *ptrData;
    char TaskName[TASK_NAME_LENGTH];
}myTask;
/**
 * @brief Aloca o numero maximo de tasks dado que o LPC80X não lida bem com a alocação dinamica de memoria*/
myTask taskVet[MAX_TASK_NUM];
/**Configura todas as tasks*/
void setupTask()
{
    for (int i = 0; i < MAX_TASK_NUM; i++)
    {
        taskVet[i].ptrFunc = NULL;
        taskVet[i].delay = 0;
        taskVet[i].lastExecution = 0;
        taskVet[i].ptrData = NULL;
    }
}
/**
 * @brief Adiciona uma nova task
 * 
 * @param ptrFunc Função que o gerenciador de task irá controlar
 * @param name Nome da task
 * @param frequency frequencia em que a task irá rodar
 * @param priority prioridade de execução
 * @param ptrData ponteiro para data utilizada na task
 * @return int 0 caso foi possivel criar a task
 */
int newTask(taskFuncType *ptrFunc, char *name, unsigned int frequency, unsigned char priority, void *ptrData)
{
    if ((!taskIniciada) && (!taskNum))
        setupTask();
    if (taskNum >= MAX_TASK_NUM)
        return -1;
    if (ptrFunc == NULL)
        return -2;
    if (frequency == 0)
        return -3;
    taskVet[taskNum].ptrFunc = ptrFunc;
    taskVet[taskNum].delay = SysFrequency/frequency;
    taskVet[taskNum].priority = priority;
    taskVet[taskNum].ptrData = ptrData;
    if (strlen(name) <= TASK_NAME_LENGTH)
        memcpy(taskVet[taskNum].TaskName,name,strlen(name));
    taskNum++;
    return 0;
}
/**
 * @brief Deleta task pelo numero dela, após isso realoca os numeros de cada task
 * 
 * @param number Numero da task que será excluida 
 * @return int  retorna 0 se foi possivel deletar a task
 *              retorna -1 se o numero está fora do intervalo das tasks
 */
int deleteTaskByNumber(int number)
{
    if (!(number<taskNum))
        return -1;
    for (int i = number; i < (MAX_TASK_NUM-1); i++)
        memcpy(&taskVet[i], &taskVet[i+1], sizeof(myTask));
    if (taskNum ==  (MAX_TASK_NUM-1))
    {  
        taskVet[(MAX_TASK_NUM-1)].ptrFunc = NULL;
        taskVet[(MAX_TASK_NUM-1)].delay = 0;
        taskVet[(MAX_TASK_NUM-1)].lastExecution = 0;
        taskVet[(MAX_TASK_NUM-1)].priority = 0;
        taskVet[(MAX_TASK_NUM-1)].ptrData = NULL;
    }
    return 0;
}
/**
 * @brief Deleta a task pelo nome
 * 
 * @param name Nome da task a ser excluida
 * @return int retona 0 se foi possivel excluir a task
 */
int deleteTaskByName(int *name)
{
    return -1;
}
/**
 * @brief Faz o controle e execução das tasks
 * 
 * @param arg 
 */
void scheduler(void *arg)
{
    if (!taskNum)
        return;
    for (int i = 0; i < taskNum; i++)
    {
        if (taskVet[i].ptrFunc != NULL)
        {
            if ((timeAux - taskVet[i].lastExecution) >= taskVet[i].delay)
            {
                taskVet[i].ptrFunc(taskVet[i].ptrData);
                taskVet[i].lastExecution = timeAux;
                return;//Pode ser removido no futuro
            }
        }
    }
}
/**
 * @brief Faz a inicialização da interrupção utilizada para controlar as tasks
 * 
 * @param SysFreq 
 */
void beginTasks(unsigned int SysFreq)
{
    taskIniciada = 1;
    setupTask();
    SysFrequency = SysFreq;
    SysTickBeginISR(SysFreq,scheduler);
}

