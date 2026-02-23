#include "myInterrupt.h"
#include "clock_config.h"
#include "board.h"


/*******************************************************************************
 * External Interrupts
 ******************************************************************************/
unsigned char intCount = 0;
ISR_FUNCTION ptrIsrFunc[8] = {NULL};

void setInterrupt(unsigned char pin, ISR_FUNCTION isrFunc, enum INTERRUPT_TRIGGER trigger)
{
    if (isrFunc==NULL)
        return;
    if (intCount >= 8)
        return;
    __disable_irq(); // turn off globally
    if (intCount == 0)
    {
        NVIC_DisableIRQ(PIN_INT0_IRQn);
        SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_GPIO_INT_MASK;//Ativa as interrupções do sistema
        SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK;// reset GPIO Interrupt (bit=0)
        SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK;// clear reset (bit=1)
    }
    for (int i = 0; i < intCount; i++)
        NVIC_DisableIRQ(PIN_INT0_IRQn+i); // turn off the PIN INT0 interrupt.
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
    ptrIsrFunc[intCount] = isrFunc;
    intCount++;
    for (int i = 0; i < intCount; i++)
        NVIC_EnableIRQ(PIN_INT0_IRQn+i);
    __enable_irq();
}
void PIN_INT0_IRQHandler(void)
{
    if (PINT->IST & (1<<0))
    {
        PINT->IST = (1<<0);
        if (ptrIsrFunc[0] != NULL)
            ptrIsrFunc[0](NULL);
    }
    return;
}
void PIN_INT1_IRQHandler(void)
{
    if (PINT->IST & (1<<1))
    {
        PINT->IST = (1<<1);
        if (ptrIsrFunc[1] != NULL)
            ptrIsrFunc[1](NULL);
    }
    return;
}
void PIN_INT2_IRQHandler(void)
{
    if (PINT->IST & (1<<2))
    {
        PINT->IST = (1<<2);
        if (ptrIsrFunc[2] != NULL)
            ptrIsrFunc[2](NULL);
    }
    return;
}
void PIN_INT3_IRQHandler(void)
{
    if (PINT->IST & (1<<3))
    {
        PINT->IST = (1<<3);
        if (ptrIsrFunc[3] != NULL)
            ptrIsrFunc[3](NULL);
    }
    return;
}
void PIN_INT4_IRQHandler(void)
{
    if (PINT->IST & (1<<4))
    {
        PINT->IST = (1<<4);
        if (ptrIsrFunc[4] != NULL)
            ptrIsrFunc[4](NULL);
    }
    return;
}
void PIN_INT5_IRQHandler(void)
{
    if (PINT->IST & (1<<5))
    {
        PINT->IST = (1<<5);
        if (ptrIsrFunc[5] != NULL)
            ptrIsrFunc[5](NULL);
    }
    return;
}
void PIN_INT6_IRQHandler(void)
{
    if (PINT->IST & (1<<6))
    {
        PINT->IST = (1<<6);
        if (ptrIsrFunc[6] != NULL)
            ptrIsrFunc[6](NULL);
    }
    return;
}
void PIN_INT7_IRQHandler(void)
{
    if (PINT->IST & (1<<7))
    {
        PINT->IST = (1<<7);
        if (ptrIsrFunc[7] != NULL)
            ptrIsrFunc[7](NULL);
    }
    return;
}

/*******************************************************************************
 * Timers Interrupts
 ******************************************************************************/
ISR_FUNCTION ptrIsrTimer[2] = {NULL};
void setTimerInterrupt(unsigned char Timer,  ISR_FUNCTION isrFunc, enum TIMER_MODE timerMode, unsigned int ticks)
{
    if (!((ticks>0)&&(isrFunc!=NULL)))
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

void setTimerIntFreq(unsigned char Timer,  ISR_FUNCTION isrFunc, long frequency)
{
    if (!((frequency>0)&&(isrFunc!=NULL)))
        return;
    long ticks = (SystemCoreClock/frequency);
    setTimerInterrupt(Timer,  isrFunc, RepeatInterrupt, (unsigned int) ticks);
}

void setTimerIntOneShot(unsigned char Timer,  ISR_FUNCTION isrFunc, long time_us)
{
    if (!((time_us>0)&&(isrFunc!=NULL)))
        return;
    long ticks = (SystemCoreClock/1000000)*time_us;
    setTimerInterrupt(Timer,  isrFunc, OneShot, (unsigned int) ticks);
}

void MRT0_IRQHandler(void) 
{
    // Remove the interrupt flag of the correct channel
    // Read the IRQ flags. Which channel to deal with?
    if (MRT0->IRQ_FLAG & 0b01) 
    { 
        // writing a bit (1) to this flag removes this IRQ.
        MRT0->CHANNEL[0].STAT = MRT_CHANNEL_STAT_INTFLAG_MASK;
        if (ptrIsrTimer[0]!=NULL)
            ptrIsrTimer[0](NULL);
    }else 
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
unsigned int SysTickFrequency = 0;
unsigned int dT_us = 0;
unsigned int dT_ms = 0;

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
    // Enable IRQs (SysTick & global)
    NVIC_EnableIRQ(SysTick_IRQn); // SysTick IRQs are on.
    __enable_irq(); // global
}
void SysTickBeginISR(int frequency,  ISR_FUNCTION isrFunc)
{
    if (!((frequency>0)&&(isrFunc!=NULL)&&(frequency<40000)))
        return;
    ptrSysTickISR = isrFunc;
    SysTickBegin(SystemCoreClock/frequency);
}
unsigned long SysTickGetTicks()
{
    return SysTickCount;
}
void SysTick_Handler(void)
{
    SysTickCount++;
    if (ptrSysTickISR!=NULL)
        ptrSysTickISR(NULL);
}

unsigned long SysTickGetTime_us()
{
    return (dT_us*SysTickCount);
}
unsigned long SysTickGetTime_ms()
{
    return ((dT_us * SysTickCount)/1000);
}
unsigned long SysTickGetTime_seconds()
{
    return SysTickGetTime_ms() / 1000;
}