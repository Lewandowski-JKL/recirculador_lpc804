#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock.h"
#include "log_lpc804.h"

#define WKT_FREQ 1000000 // Use if the WKT is clocked by the LPOSC
//#define WKT_FREQ 937500 // Use if the WKT is clocked by the 15 MHz FRO,
// via the div-by-16 WKT divider
#define WKT_RELOAD 1667 // Reload value for the WKT down counter
// 1000000 means 2 Hz LED freq (1 Hz IRQ)
// 1667 means 300 Hz GPIO toggle (600 Hz IRQ)
#define WKT_INT_FREQ (WKT_FREQ/WKT_RELOAD) // Interrupt frequency of the WKT.
#define TXD_OUTPUT (4)// PIO0_4 is connected to the debugger's UART receive pin

void initDebug()
{
    __disable_irq();
    // -------------- Step 1: Turn on the USART0 ---------------------
    // Bit is 14 (USART0). Set to 1 to turn on USART0.
    // Optionally, we could also turn on USART1 with bit 15
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_UART0_MASK); // Set bit 14.
    // -------------- Step 2: Reset the USART0 ---------------------
    // Set bit 14 to 0: assert (i.e. "make") the USART0 reset
    // Set bit 14 to 1: remove the USART0 reset
    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_UART0_RST_N_MASK); // Reset USART0
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_UART0_RST_N_MASK); // remove the reset
    // ------ Step 3: Connect PIO0_4 & _0 to USART0 via Switch Matrix -------------
    // The '802 is designed to have a USART route to a PC over USB by taking
    // advantage of the onboard debugger and connecting to it first. The debugger
    // then routes the TXD and RXD UART lines in a way that make them appear as a
    // virtual serial port to the PC over USB.
    //
    // Out-of-the-box, the schematic (pg 3) shows us that the '802 PIO0_4 is supposed to be
    // TXD and PIO0_0 is supposed to be RXD, but only if the JP5 and JP6 jumpers are installed.
    // Check the board... they are. Alternatively, if you remove JP5 and JP6 jumpers
    // and solder-bridge (or install 0 Ohm resistors) R37 and R38 you can use
    // PIO0_8 as RXD and PIO0_9 as TXD.
    //
    // Switch Matrix PINASSIGN0 is responsible for RXD (Bits 15:8) and TXD (Bits 7:0)
    // Some other Pin assignments for USART0 can be found in PINASSIGN1, along with
    // mux settings for USART1.
    // enable switch matrix
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    // Set switch matrix
    // Clear bits 15:0
    SWM0->PINASSIGN.PINASSIGN0 &= ~(SWM_PINASSIGN0_U0_TXD_O_MASK | SWM_PINASSIGN0_U0_RXD_I_MASK);
    // Assign TXD and RXD ports to PINASSIGN0 bits 15:0
    // TXD is PIO0_4, so put 4 into bits 7:0
    // RXD is PIO0_0, so put 0 into bits 15:8
    SWM0-> PINASSIGN.PINASSIGN0 |= ( (0x4UL<<SWM_PINASSIGN0_U0_TXD_O_SHIFT) | // TXD is PIO0_4 into 7:0
    (0x0UL<<SWM_PINASSIGN0_U0_RXD_I_SHIFT)); // RXD is PIO0_0
    // USART0 is now set to PIO0_4 for TX and PIO0_0 for RX.
    // disable the switch matrix
    SYSCON->SYSAHBCLKCTRL0 &= ~(SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    // ---------------- End of Switch Matrix code -----------------------------------
    // -------------- Step 4: Configure USART0 Clock ---------------------
    // Use FRG as the input to the USART's BRG
    SYSCON->UART0CLKSEL = 0x02;// FRG is 0b010
    // Step 4a. Choose clock source for clock, stage 1: FRG0
    // both FRO and main clock have been set earlier.
    //usually, FRO is 12 Mhz or 15 Mhz.
    SYSCON->FRG[0].FRGCLKSEL = 0x00;// Choose FRO (0x0) or Main Clock (0x1)
    // Step 4b. Set the fractional clock divider and multiplier pre-scaler values
    // In synchronous mode FCLK = (FRGINPUTCLK)/(1+(MULT/DIV)
    // or MULT = DIV*((FRGINPUTCLK/FCLK)-1)
    // FRGINPUTCLK is 12 MHz and FCLK is the desired data rate and DIV is 256.
    SYSCON->FRG[0].FRGDIV = 0xFF;// DIV is always 255 (0xFF); internally, it will be the programmed value PLUS ONE. So 255 becomes 256 in the calculation. divisor value is DIV+1 (so 255+1 = 256); according to user manual (6.6.14; Table 67)
    SYSCON->FRG[0].FRGMULT = 244;// MULT is 0 to 255. Unlike DIV, the programmed value is the ACTUAL value used in the calculation.
    // Step 4b. Choose the BRG value
    // If 0, then the output of the BRG is same freq as input freq. (divisor: BRG+1=1)
    // If 1, then output of BRG is divided by 2 (divisor: BRG+1=2)
    USART0->BRG = 1279; 
    // Oversampling within the Baud Rate Generator.
    // Default is 15 (0xF ) out of reset, making it oversample OSR+1 (16) times.
    // Recommend it remain 16 times over-sampling as per NXP recommendations.
    // but it can be tweaked down to OSR=4 to allow 5 times oversampling.
    // Cannot go below OSR=4 and cannot go above OSR=15.
    USART0->OSR = 0xF;// Oversample at OSR+1 = 16 times.
    // USART CONFIG (only one bit to set here, really: data length)
    USART0->CFG |= ( 0x1<<USART_CFG_DATALEN_SHIFT | // Data length: 8
    0x0<<USART_CFG_PARITYSEL_SHIFT | // Parity: none
    0x0<<USART_CFG_STOPLEN_SHIFT); // 1 stop bit.
    // nothing to be done here.
    USART0->CTL = 0;
    // clear any pending flags, just in case something happened.
    USART0->STAT = 0xFFFF;
    // Enable the USART0
    USART0->CFG |= USART_CFG_ENABLE_MASK;// set bit 1 to 1.
    // -------------- Step 5: Enable interrupts ---------------------
    __enable_irq(); // turn on globally

}
void endDebug()
{

}
void SerialPrint(char string[])
{
    int i = 0;
    while (i<256)
    {
        if (string[i] == '\0')
        {
            SerialPrintln(string, i);
            return;
        }
        i++;
    }
}
void SerialPrintln(char message[], unsigned char len)
{
    int i = 0;
    while (i<len)
    {
        while(!((USART0->STAT) & USART_STAT_TXRDY_MASK));// wait for TX ready
        USART0->TXDAT = message[i];// place in transmit buffer
        i++;
    }
}