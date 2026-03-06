#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========= Configurações ========= */

/* Número máximo de tarefas (exclui a idle, que é interna) */
#ifndef TASK_MAX_TASKS
#define TASK_MAX_TASKS  8
#endif

/* Frequência do tick (Hz). Use 1000 para ms exatos. */
#ifndef TASK_TICK_HZ
#define TASK_TICK_HZ    1000u
#endif

/* Tamanho da stack da idle (em palavras de 32 bits). */
#ifndef TASK_IDLE_STACK_WORDS
#define TASK_IDLE_STACK_WORDS  64
#endif

/* ========= Tipos e API ========= */

typedef void (*task_entry_t)(void *arg);

typedef enum {
    TASK_READY = 0,
    TASK_SLEEPING,
    TASK_TERMINATED
} task_state_t;

/* TCB — precisa ter 'sp' como primeiro campo (usado no assembly). */
typedef struct task_t {
    uint32_t    *sp;          /* <--- precisa ser o primeiro campo */
    task_entry_t entry;
    void        *arg;
    uint32_t     wake_tick;
    uint8_t      state;
    uint8_t      id;
    const char  *name;
} task_t;

/* Inicializa o mini-scheduler (cria a idle internamente). */
void task_system_init(void);

/* Cria uma tarefa.
 * - 'stack_mem' deve apontar para uma memória de pilha (RAM) do usuário.
 * - 'stack_bytes' é o tamanho em bytes dessa pilha.
 * Retorna: índice/ID (>=0) ou -1 se falhar.
 */
int  task_create(task_entry_t entry, void *arg,
                 void *stack_mem, size_t stack_bytes,
                 const char *name);

/* Inicia o agendador: transfere o controle para a primeira tarefa 'READY'.
 * Não retorna.
 */
void task_start(void);

/* Entrega voluntariamente a CPU para a próxima tarefa pronta. */
void task_yield(void);

/* Dorme pelo tempo em milissegundos (cooperativo). */
void task_delay(uint32_t delay_ms);

/* Marca a tarefa atual como terminada (nunca retorna). */
void task_exit(void);

/* Função a ser chamada no ISR do timer/SysTick, a cada 1 tick. */
void task_tick_isr(void);

/* (Opcional) Obtém o ID da tarefa atual. */
int  task_current_id(void);

/* (Opcional) Ticks do sistema desde o start. */
uint32_t task_ticks(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_H */