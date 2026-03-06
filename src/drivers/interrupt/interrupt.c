/**
 * @file myInterrupt.c
 * @author Jackson Kenedi Lewandowski (jackson.lewandowski@komlog.com/jacksonlewandowski.work@gmail.com)
 * @brief Essa biblioteca faz a gestão das interrupções externas e internas
 * @version 0.1
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "interrupt.h"
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * External Interrupts
 ******************************************************************************/
unsigned char intCount = 0;//Contador de interrupções externas
ISR_FUNCTION ptrIsrFunc[8] = {NULL};//ponteiro para funções utilizada em interrupções externas
/**
 * @brief Cria uma nova interrupção externa
 * 
 * @param pin       pino onde ira acontecer a interrupção
 * @param isrFunc   função que será chamada com essa interrupção
 * @param trigger   Tipo de interrupção externa -> falling, rising, edge, low level ou high level
 */
void newExternInterrupt(unsigned char pin, ISR_FUNCTION isrFunc, enum INTERRUPT_TRIGGER trigger)
{
    if (isrFunc==NULL)//Verifica se o ponteiro para função está alocado
        return;
    if (intCount >= 8)//verifica se não estourou o numero de interrupções
        return;
    __disable_irq(); // turn off globally
    if (intCount == 0)//No caso de não ter nenhuma interrupção definida
    {
        NVIC_DisableIRQ(PIN_INT0_IRQn);//desabilita interrupção '0'
        SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_GPIO_INT_MASK;//Ativa as interrupções do sistema
        SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK;// reset GPIO Interrupt (bit=0)
        SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK;// clear reset (bit=1)
    }
    for (int i = 0; i < intCount; i++)
        NVIC_DisableIRQ(PIN_INT0_IRQn+i); //desabilita as interrupções externas
    //configura o canal de interrupcao para o pino escolhido
    SYSCON->PINTSEL[intCount] = pin;
    //configura borda (0) ou nível (1)
    PINT->ISEL |= (((trigger & 0b100) >> 2) << intCount);
    //Borda de decida/nivel baixo
    PINT->SIENF |= ((trigger & 0b01) << intCount);
    PINT->CIENF |= (((~trigger)&0b01) << intCount);
    //borda de subida/nivel alto
    PINT->SIENR |= (((trigger & 0b10)>>1) << intCount);
    PINT->CIENR |= ((((~trigger)&0b10)>>1) << intCount);
    //limpa todas flags de interrupção pendentes
    PINT->IST = 0xFF;
    ptrIsrFunc[intCount] = isrFunc; //Aponta para a função que executara a resolução da interrupção
    intCount++; //Incrementa ocntador de interrupções
    for (int i = 0; i < intCount; i++)
        NVIC_EnableIRQ(PIN_INT0_IRQn+i);//Habilita todas as interrupções
    __enable_irq();//termina de habilitar
}
/**
 * @brief Função que trata a interrupção externa '0'
 * 
 */
void PIN_INT0_IRQHandler(void)
{
    if (PINT->IST & (1<<0))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<0);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[0] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[0](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '1'
 * 
 */
void PIN_INT1_IRQHandler(void)
{
    if (PINT->IST & (1<<1))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<1);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[1] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[1](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '2'
 * 
 */
void PIN_INT2_IRQHandler(void)
{
    if (PINT->IST & (1<<2))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<2);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[2] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[2](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '3'
 * 
 */
void PIN_INT3_IRQHandler(void)
{
    if (PINT->IST & (1<<3))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<3);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[3] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[3](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '4'
 * 
 */
void PIN_INT4_IRQHandler(void)
{
    if (PINT->IST & (1<<4))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<4);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[4] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[4](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '5'
 * 
 */
void PIN_INT5_IRQHandler(void)
{
    if (PINT->IST & (1<<5))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<5);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[5] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[5](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '6'
 * 
 */
void PIN_INT6_IRQHandler(void)
{
    if (PINT->IST & (1<<6))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<6);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[6] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[6](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/**
 * @brief Função que trata a interrupção externa '7'
 * 
 */
void PIN_INT7_IRQHandler(void)
{
    if (PINT->IST & (1<<7))//Verifica a flag de interrupção
    {
        PINT->IST = (1<<7);//Limpa a flag de interrupção
        //Nota: O registrador PINT->IST funciona como um "clear-on-write", ou seja,
        //quando escrever 1 em um bit específico isso fara com que a limpe a flag
        //(interrupção) nesse bit. Essa interpretação é feita pelo hardware resultando 
        //em um processo eficiente pois permite utilizar o mesmo registrador para
        //verificar e limpara a interrupção.
        if (ptrIsrFunc[7] != NULL)//Verifica se existe uma função para ser chamada
            ptrIsrFunc[7](NULL);//Chama a função de tratamento de interrupção
    }
    return;
}
/*******************************************************************************
 * Timers Interrupts
 ******************************************************************************/
ISR_FUNCTION ptrIsrTimer[2] = {NULL};//Ponteiro para funções utilizadas nas interrupções de timer
/**
 * @brief Cria uma interrupção de timer
 * 
 * @param Timer Define se será o timer 0 ou o timer 1
 * @param isrFunc Função que fara o tratamento da interrupção
 * @param timerMode O tipo de interrupção, se repetira ou se será uma vez apenas
 * @param ticks Numero de ticks que irá levar para ocorrer a interrupção
 */
void newTimerInterrupt(unsigned char Timer,  ISR_FUNCTION isrFunc, enum TIMER_MODE timerMode, unsigned int ticks)
{
    //Para mais detalhes
    //https://www.yorku.ca/professor/drsmith/2020/07/13/lpc802_course/
    if (!((ticks>0)&&(isrFunc!=NULL)))//Verifica se o numero de ticks é maior que zero e se o ponteiro para função não é nulo
        return;
    __disable_irq(); // turn off globally
    NVIC_DisableIRQ(MRT0_IRQn); // turn off the MRT0 interrupt
    // ----------------------- Begin Core Clock Select -----------------------
    // Specify that we will use the Free-Running Oscillator
    // Set the main clock to be the FRO
    // 0x0 is FRO; 0x1 is external clock ; 0x2 is Low pwr osc.; 0x3 is FRO_DIV
    // Place in bits 1:0 of MAINCLKSEL.
    //SYSCON->MAINCLKSEL = (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    // Update the Main Clock
    // Step 1. write 0 to bit 0 of this register
    // Step 2. write 1 to bit 0 this register
    //SYSCON->MAINCLKUEN &= ~(0x1); // step 1. (Sec. 6.6.4 of manual)
    //SYSCON->MAINCLKUEN |= 0x1; // step 2. (Sec. 6.6.4 of manual)
    // Set the FRO frequency (clock_config.h in SDK)
    // For FRO at 9MHz: BOARD_BootClockFRO18M();
    // 12MHz: BOARD_BootClockFRO24M();
    // 15MHz: BOARD_BootClockFRO30M();
    // See: Section 7.4 User Manual
    // This is more complete than just using set_fro_frequency(24000); for 12 MHz
    //BOARD_BootClockFRO30M(); // 30M, 24M or 18M for 15MHz, 12MHz or 9MHz
    // turn on the MRT
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_MRT_MASK);// MRT is on (bit 10 set)
    // Reset the MRT
    // Put 0 in the MRT reset bit to reset it.
    // Then put a 1 in the MRT reset bit to allow the MRT to operate.
    // manual: Section 6.6.10
    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_MRT_RST_N_MASK);// reset MRT (bit=0)
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_MRT_RST_N_MASK);// clear reset (bit=1)
    // Put Channel 0 & Channel 1 into one-shot interrupt mode.
    // Mode = repeat, interrupt = enable
    // Bit 0: INTEN is 1 for interrupt enabled
    // Bits 2:1: MODE: 0x0 for repeat mode (0x1 for one-shot int; 0x2 for one-shot stall)
    MRT0->CHANNEL[Timer].CTRL = (timerMode << MRT_CHANNEL_CTRL_MODE_SHIFT | MRT_CHANNEL_CTRL_INTEN_MASK);
    // Note: to check to see if the timer is running:
    // Bit 1 in STAT register is 1 if timer is running; 0 if it is NOT running
    MRT0->CHANNEL[Timer].INTVAL = ticks & ~(MRT_CHANNEL_INTVAL_LOAD_MASK); // Clear bit 31
    ptrIsrTimer[Timer] = isrFunc;
    NVIC_EnableIRQ(MRT0_IRQn);
    __enable_irq(); // global
}
/**
 * @brief Cria uma interrupção de timer que repete
 * 
 * @param Timer timer onde irá ocorrer a interrupção Timer 0 ou Timer 1
 * @param isrFunc Função que irá tratar a interrupção
 * @param frequency Frequencia da interrupção
 */
void newTimerIntFreq(unsigned char Timer,  ISR_FUNCTION isrFunc, long frequency)
{
    if (!((frequency>0)&&(isrFunc!=NULL)))
        return;
    long ticks = (SystemCoreClock/frequency);//Calcula o numero de Ticks necessário de acordo com o clock utilizado
    newTimerInterrupt(Timer,  isrFunc, RepeatInterrupt, (unsigned int) ticks);//Cria a interrupção
}
/**
 * @brief Cria uma interrupção de timer que ocorre apenas uma vez
 * 
 * @param Timer timer onde irá ocorrer a interrupção Timer 0 ou Timer 1
 * @param isrFunc Função que irá tratar a interrupção
 * @param time_us Tempo em us que levará para ocorrer a interrupção
 */
void newTimerIntOneShot(unsigned char Timer,  ISR_FUNCTION isrFunc, long time_us)
{
    if (!((time_us>0)&&(isrFunc!=NULL)))
        return;
    long ticks = (SystemCoreClock/1000000)*time_us;//Calcula o numero de ticks necessário para que ocorra a interrupção
    newTimerInterrupt(Timer,  isrFunc, OneShot, (unsigned int) ticks);//Cria a interrupção
}
/**
 * @brief Função que trata a interrupção de timer
 * 
 */
void MRT0_IRQHandler(void) 
{
    // Remove the interrupt flag of the correct channel
    // Read the IRQ flags. Which channel to deal with?
    if (MRT0->IRQ_FLAG & (0b01<<0))
    { 
        // writing a bit (1) to this flag removes this IRQ.
        MRT0->CHANNEL[0].STAT = MRT_CHANNEL_STAT_INTFLAG_MASK;
        if (ptrIsrTimer[0]!=NULL)
            ptrIsrTimer[0](NULL);
    }
    if (MRT0->IRQ_FLAG & (0b01<<1))
    {   
        // writing a bit (1) to this flag removes this IRQ.
        MRT0->CHANNEL[1].STAT = MRT_CHANNEL_STAT_INTFLAG_MASK;
        if (ptrIsrTimer[1]!=NULL)
            ptrIsrTimer[1](NULL);
    }
}

/*******************************************************************************
 * SysTick Interrupt
 ******************************************************************************/
ISR_FUNCTION ptrSysTickISR = NULL;
volatile unsigned long SysTickCount = 0;
volatile unsigned long SysTickFrequency = 0;
volatile unsigned long dT_us = 0;
volatile unsigned long dT_ms = 0;
/**
 * @brief Configura uma interrupção continua para controle do sistema
 * 
 * @param ticks 
 */
void SysTickBegin(int ticks)
{
    if (!(ticks>0))
        return;
    // disable interrupts (global (all) and SysTick (specific))
    __disable_irq(); // turn off globally
    NVIC_DisableIRQ(SysTick_IRQn); // turn off the SysTick interrupt.
    // ----------------------- Begin Core Clock Select -----------------------
    // Specify that we will use the Free-Running Oscillator
    // Set the main clock to be the FRO. 0x0 is FRO; 0x1 is external clock; 
    // 0x2 is Low pwr osc.; 0x3 is FRO_DIV. Place in bits 1:0 of MAINCLKSEL.
    SYSCON->MAINCLKSEL = (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    // Update the Main Clock
    // Step 1. write 0 to bit 0 of this register
    // Step 2. write 1 to bit 0 this register
    SYSCON->MAINCLKUEN &= ~(0x1); // step 1. (Sec. 6.6.4 of manual)
    SYSCON->MAINCLKUEN |= 0x1; // step 2. (Sec. 6.6.4 of manual)
    // Set the FRO frequency (clock_config.h in SDK)
    // For FRO at 9MHz: BOARD_BootClockFRO18M(); 12MHz: BOARD_BootClockFRO24M();
    // 15MHz: BOARD_BootClockFRO30M();
    // See: Section 7.4 User Manual
    //BOARD_BootClockFRO24M(); // 30M, 24M or 18M for 15MHz, 12MHz or 9MHz
    // ----------------------- End of Core Clock Select -----------------------
    // ----------------------- Begin SysTick setup ------------------------
    // Configure SysTick to fire once every 0.2 seconds.
    // Argument is the number of clock ticks between IRQs.
    //
    // Clock rate: 12 MHz via FRO. (8.3*10^(-8) sec period)
    // 12,000,000 ticks/sec * 0.2 sec = 2400000 ticks
    // SysTick is a 24 bit timer (max almost 17 million)
    SysTickFrequency = SystemCoreClock/ticks;
    dT_us = 1000000/SysTickFrequency;
    SysTick_Config(ticks); // 2400000 ticks = 0.2 sec @ 12 MH
//     if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
//         return;                                                   /* Reload value impossible */


//   SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
//   NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
//   SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
//   SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
//                    SysTick_CTRL_TICKINT_Msk   |
//                    SysTick_CTRL_ENABLE_Msk;      

    // Enable IRQs (SysTick & global)
    NVIC_EnableIRQ(SysTick_IRQn); // SysTick IRQs are on.
    __enable_irq(); // global
}
/**
 * @brief Cria uma interrupção para controle do sistema
 * 
 * @param frequency frequencia de interrupção
 * @param isrFunc   função que irá tratar a interrupção
 */
void SysTickBeginISR(int frequency,  ISR_FUNCTION isrFunc)
{
    if (!((frequency>0)&&(isrFunc!=NULL)&&(frequency<40000)))
        return;
    ptrSysTickISR = isrFunc;
    SysTickBegin(SystemCoreClock/frequency);
}
/**
 * @brief Retorna o numero de interrupções que o sistema ja teve
 * 
 * @return unsigned long 
 */
unsigned long SysTickGetTicks()
{
    return SysTickCount;
}
/**
 * @brief Função de tratamento de interrupção
 * 
 */
void SysTick_Handler(void)
{
    SysTickCount++;
    if (ptrSysTickISR!=NULL)
        ptrSysTickISR(&SysTickCount);
}
/**
 * @brief Retorna o tempo em us
 * 
 * @return unsigned long 
 */
unsigned long SysTickGetTime_us()
{
    return (dT_us*SysTickCount);
}
/**
 * @brief Retorna o tempo em mili segundos
 * 
 * @return unsigned long 
 */
unsigned long SysTickGetTime_ms()
{
    return ((dT_us * SysTickCount)/1000);
}
/**
 * @brief Retorna o tempo em segundos
 * 
 * @return unsigned long 
 */
unsigned long SysTickGetTime_seconds()
{
    return SysTickGetTime_ms() / 1000;
}
