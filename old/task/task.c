#include "task.h"
#include <string.h>

/* =================== Estado interno =================== */

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

/* Lista interna de tarefas (exclui a idle, que tem TCB próprio) */
static task_t   g_tasks[TASK_MAX_TASKS];
static int      g_task_count = 0;

/* Tarefa atual e índice atual (para round-robin simples) */
static task_t  *g_cur = NULL;
static int      g_cur_index = -1;

/* Ticks do sistema */
static volatile uint32_t g_ticks = 0;

/* Idle (TCB + stack internos) */
static task_t    g_idle_tcb;
static uint32_t  g_idle_stack[TASK_IDLE_STACK_WORDS];

/* Protótipos internos */
static void idle_task(void *arg);
static void task_trampoline(void);
static int  pick_next_ready_after(int start_index);
static void make_initial_stack(task_t *tcb, void *stack_mem, size_t stack_bytes);

/* ========= Rotinas de troca de contexto (ASM Cortex-M0/M0+) =========
 *
 * Convenções:
 *  - Salvamos r4-r7, e copiamos r8-r11 em r4-r7 para salvar também.
 *  - Salvamos/restauramos LR (retorno).
 *  - r0-r3,r12 são caller-saved (podem ser clobbered entre yields), o que é OK.
 *
 * Assinaturas:
 *  void ctx_switch(task_t **ppCur, task_t *next);
 *  void ctx_launch(task_t *next);
 *
 * Observação: 'naked' evita prólogo/epílogo do compilador.
 */

/* Troca de contexto entre tarefas já iniciadas */
__attribute__((naked))
static void ctx_switch(task_t **ppCur, task_t *next)
{
    __asm volatile(
        /* Salva r4-r7 */
        "push   {r4-r7}           \n"
        /* Salva r8-r11 (via r4-r7) */
        "mov    r4, r8            \n"
        "mov    r5, r9            \n"
        "mov    r6, r10           \n"
        "mov    r7, r11           \n"
        "push   {r4-r7}           \n"
        /* Salva LR */
        "push   {lr}              \n"

        /* *ppCur -> r3 ; salva SP em (*ppCur)->sp */
        "ldr    r3, [r0]          \n" /* r0 = ppCur */
        "str    sp, [r3]          \n" /* (*ppCur)->sp = sp */

        /* *ppCur = next */
        "str    r1, [r0]          \n" /* r1 = next */

        /* Carrega SP da próxima */
        "ldr    sp, [r1]          \n" /* sp = next->sp */

        /* Restaura de volta: LR, r8-r11, r4-r7 e retorna */
        "pop    {lr}              \n"
        "pop    {r4-r7}           \n"
        "mov    r8, r4            \n"
        "mov    r9, r5            \n"
        "mov    r10, r6           \n"
        "mov    r11, r7           \n"
        "pop    {r4-r7}           \n"
        "bx     lr                \n"
    );
}

/* Lança a primeira tarefa (não retorna). */
__attribute__((naked))
static void ctx_launch(task_t *next)
{
    __asm volatile(
        "ldr    sp, [r0]          \n" /* sp = next->sp  (r0 = next) */
        /* Restaura contexto empilhado pela make_initial_stack() */
        "pop    {lr}              \n"
        "pop    {r4-r7}           \n"
        "mov    r8, r4            \n"
        "mov    r9, r5            \n"
        "mov    r10, r6           \n"
        "mov    r11, r7           \n"
        "pop    {r4-r7}           \n"
        "bx     lr                \n"
    );
}

/* ================== Implementação do scheduler ================== */

void task_system_init(void)
{
    /* Zera lista de tarefas */
    memset(g_tasks, 0, sizeof(g_tasks));
    g_task_count = 0;
    g_cur = NULL;
    g_cur_index = -1;
    g_ticks = 0;

    /* Cria idle internamente (stack própria) */
    memset(&g_idle_tcb, 0, sizeof(g_idle_tcb));
    g_idle_tcb.entry = idle_task;
    g_idle_tcb.arg   = NULL;
    g_idle_tcb.state = TASK_READY;
    g_idle_tcb.id    = -1;
    g_idle_tcb.name  = "idle";
    make_initial_stack(&g_idle_tcb, g_idle_stack, sizeof(g_idle_stack));
}

/* Monta a pilha inicial de uma tarefa para que o primeiro 'restore' pule para task_trampoline() */
static void make_initial_stack(task_t *tcb, void *stack_mem, size_t stack_bytes)
{
    /* Alinhamento e topo da stack (stack cresce para baixo) */
    uint32_t *sp = (uint32_t *)((uintptr_t)stack_mem + stack_bytes);
    sp = (uint32_t *)(((uintptr_t)sp) & ~0x7u); /* alinhar em 8 bytes */

    /* Layout que ctx_launch()/ctx_switch() esperam na ordem inversa dos pops:
     *
     *   [  r4 ]  (baixo)   <-- último pop
     *   [  r5 ]
     *   [  r6 ]
     *   [  r7 ]
     *   [ r8' ]  (será movido p/ r8)
     *   [ r9' ]
     *   [ r10']
     *   [ r11']
     *   [  LR ]  (primeiro pop)
     *
     * Valores iniciais não importam, exceto LR que deve apontar para task_trampoline().
     */

    *(--sp) = (uint32_t)task_trampoline; /* LR */

    /* r8'..r11' (armazenados via r4-r7 temporários) */
    *(--sp) = 0u; /* r11' */
    *(--sp) = 0u; /* r10' */
    *(--sp) = 0u; /* r9'  */
    *(--sp) = 0u; /* r8'  */

    /* r4..r7 */
    *(--sp) = 0u; /* r7 */
    *(--sp) = 0u; /* r6 */
    *(--sp) = 0u; /* r5 */
    *(--sp) = 0u; /* r4 */

    tcb->sp   = sp;
    tcb->wake_tick = 0;
    if (tcb->state != TASK_SLEEPING && tcb->state != TASK_TERMINATED)
        tcb->state = TASK_READY;
}

/* Cria e registra uma tarefa no array interno */
int task_create(task_entry_t entry, void *arg,
                void *stack_mem, size_t stack_bytes,
                const char *name)
{
    if (!entry || !stack_mem || stack_bytes < 64) {
        return -1;
    }
    if (g_task_count >= TASK_MAX_TASKS) {
        return -1;
    }

    task_t *tcb = &g_tasks[g_task_count];
    memset(tcb, 0, sizeof(*tcb));

    tcb->entry = entry;
    tcb->arg   = arg;
    tcb->state = TASK_READY;
    tcb->id    = (uint8_t)g_task_count;
    tcb->name  = name ? name : "task";

    make_initial_stack(tcb, stack_mem, stack_bytes);

    g_task_count++;
    return (int)(tcb->id);
}

/* Seleciona próxima tarefa READY após um índice (round-robin).
 * Se nenhuma READY, retorna -1 (o que fará ir para a idle). */
static int pick_next_ready_after(int start_index)
{
    if (g_task_count == 0) return -1;

    int n = g_task_count;
    for (int k = 1; k <= n; k++) {
        int idx = (start_index + k) % n;
        if (g_tasks[idx].state == TASK_READY)
            return idx;
    }
    return -1;
}

/* Função chamada pelo timer/SysTick ISR a cada 1 tick (TASK_TICK_HZ). */
void task_tick_isr(void)
{
    g_ticks++;

    /* Acorda quem venceu delay */
    for (int i = 0; i < g_task_count; i++) {
        if (g_tasks[i].state == TASK_SLEEPING) {
            if (g_ticks >= g_tasks[i].wake_tick) {
                g_tasks[i].state = TASK_READY;
            }
        }
    }
}

/* Inicia o escalonador: escolhe uma READY e transfere controle.
 * Não retorna. Se não houver READY, vai para Idle. */
void task_start(void)
{
    /* Define tarefa atual (READY) ou idle se nenhuma */
    int next_idx = pick_next_ready_after(-1);
    if (next_idx < 0) {
        g_cur = &g_idle_tcb;
        g_cur_index = -1;
        ctx_launch(g_cur); /* nunca retorna */
    } else {
        g_cur = &g_tasks[next_idx];
        g_cur_index = next_idx;
        ctx_launch(g_cur); /* nunca retorna */
    }

    /* Segurança: nunca deveria chegar aqui */
    while (1) { /* trap */ }
}

/* Entrega a CPU voluntariamente para a próxima READY (ou idle). */
void task_yield(void)
{
    /* Decide próximo */
    int next_idx = pick_next_ready_after(g_cur_index);
    task_t *next;

    if (next_idx < 0) {
        next = &g_idle_tcb;
    } else {
        next = &g_tasks[next_idx];
    }

    if (next == g_cur) {
        /* Nada a fazer */
        return;
    }

    /* Troca de contexto:
     *  - atual: g_cur
     *  - próximo: next
     *  - atualiza g_cur no próprio assembly (via *ppCur) */
    ctx_switch(&g_cur, next);

    /* Ao retornar, já estamos no contexto (pilha) da tarefa retomada.
     * Precisamos atualizar g_cur_index coerentemente. */
    if (g_cur == &g_idle_tcb) {
        g_cur_index = -1;
    } else {
        g_cur_index = g_cur->id;
    }
}

/* Delay cooperativo em milissegundos */
void task_delay(uint32_t delay_ms)
{
    if (!g_cur) return;

    /* Converte ms -> ticks (round up) */
    uint32_t ticks = (delay_ms * (uint32_t)TASK_TICK_HZ + 999u) / 1000u;
    if (ticks == 0) ticks = 1;

    g_cur->wake_tick = g_ticks + ticks;
    g_cur->state     = TASK_SLEEPING;

    task_yield();
}

/* Tarefa atual termina: marca TERMINATED e troca para próxima. */
void task_exit(void)
{
    if (!g_cur) while (1) { /* trap */ }

    g_cur->state = TASK_TERMINATED;

    /* Escolhe próxima READY, senão vai para idle */
    int next_idx = pick_next_ready_after(g_cur_index);
    task_t *next = (next_idx < 0) ? &g_idle_tcb : &g_tasks[next_idx];

    /* Não volta */
    ctx_switch(&g_cur, next);

    /* Segurança */
    while (1) { /* trap */ }
}

/* ================== Utilidades ================== */

int task_current_id(void)
{
    return (g_cur == &g_idle_tcb || !g_cur) ? -1 : (int)g_cur->id;
}

uint32_t task_ticks(void)
{
    return g_ticks;
}

static void idle_task(void *arg)
{
    UNUSED(arg);
    for (;;) {
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
        __asm volatile("wfi");
#else
        /* Fallback se 'wfi' não existir */
        __asm volatile("nop");
#endif
    }
}

/* Trampolim: primeira vez que a tarefa roda, entra aqui.
 * Pega 'g_cur' (já aponta para o TCB certo), chama a entry,
 * e, se a função retornar, finaliza a tarefa. */
static void task_trampoline(void)
{
    task_t *self = g_cur;
    self->entry(self->arg);
    task_exit(); /* não retorna */
}