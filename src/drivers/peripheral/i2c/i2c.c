#include "i2c.h"
#include <stdlib.h>
#include "board.h"
#include "interrupt.h"
#include "checksum.h"

#define loopWaitReceive 20
#define loopWaitSend 20

#define PRIMARY_STATE_MASK (0x7<<1)
#define I2C_STAT_MSTST_IDLE ((0b000)<<1) // Main Idle: LPC802 user manual table 187
#define I2C_STAT_MSTST_RXRDY ((0b001)<<1) // Main Receive Ready " "
#define I2C_STAT_MSTST_TXRDY ((0b010)<<1) // Main Transmit Ready " "
#define I2C_STAT_MSTST_NACK_ADD ((0b011)<<1)// Main Ack Add " "
#define I2C_STAT_MSTST_NACK_DATA ((0b100)<<1)// Main Ack signal data ” ”
// Define values for I2C registers that aren't in the header file.
// Table 195 of LPC802 User Manual
#define MSTCTL_CONTINUE (1UL << 0) // Bit 0 of MSTCTL set ("Main" or "Primary")
#define MSTCTL_START (1UL << 1) // Bit 1 of MSTCTL set ("Main" or "Primary")
#define MSTCTL_STOP (1UL << 2) // Bit 2 of MSTTCL set ("Main" or "Primary")
#define CTL_SLVCONTINUE (1UL << 0) // Bit 0: Secondary level (SLV) Continue
#define CTL_SLVNACK (1UL << 1) // Bit 1: Secondary Level (SLV) Acknowledge

unsigned int I2C_mode = 0;

void _I2C_PrimarySetBaudRate(unsigned int baudRate_Bps, unsigned int srcClock_Hz);
/*******************************************************************************
 * Slave
 ******************************************************************************/
/**
 * @brief Starts registers for I2C communication in slave mode
 * 
 * @param addr Slave ADDR
 * @param sda SDA Pin
 * @param scl SCL Pin
 * @param baudRate Clock Frequency
 */
void i2cBegin(unsigned char addr, unsigned char sda, unsigned char scl, unsigned int baudRate, enum I2cMode mode)
{
    __disable_irq(); // turn off globally
    NVIC_DisableIRQ(I2C0_IRQn);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SWM0->PINASSIGN.PINASSIGN5 &= ~(SWM_PINASSIGN5_I2C0_SCL_IO_MASK | 
                                    SWM_PINASSIGN5_I2C0_SDA_IO_MASK); 
    SWM0->PINASSIGN.PINASSIGN5 |= ((scl<<SWM_PINASSIGN5_I2C0_SCL_IO_SHIFT)|
                                    (sda<<SWM_PINASSIGN5_I2C0_SDA_IO_SHIFT));

    SYSCON->SYSAHBCLKCTRL0 &= ~(SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_I2C0_MASK );// I2C is on
    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_I2C0_RST_N_MASK );// reset I2C(bit = 0)
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_I2C0_RST_N_MASK);// remove i2C reset (bit = 1)
    SYSCON->I2C0CLKSEL = 0b000;

    if (mode&0b01)
    {
        I2C0->CFG = I2C_CFG_SLVEN_MASK;
        I2C0->SLVADR[0] = (addr<<1) | 0b0;
    }
    if (mode&0b10)
    {
        I2C0->CFG = (I2C_CFG_MSTEN_MASK);
        _I2C_PrimarySetBaudRate(baudRate, SystemCoreClock);
    }
    I2C_mode = mode;
    NVIC_EnableIRQ(I2C0_IRQn);
    __enable_irq();
}
void i2cBeginSlave(unsigned char addr, unsigned char sda, unsigned char scl, unsigned int baudRate)
{
    __disable_irq(); // turn off globally
    NVIC_DisableIRQ(I2C0_IRQn);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SWM0->PINASSIGN.PINASSIGN5 &= ~(SWM_PINASSIGN5_I2C0_SCL_IO_MASK | 
                                    SWM_PINASSIGN5_I2C0_SDA_IO_MASK); 
    SWM0->PINASSIGN.PINASSIGN5 |= ((scl<<SWM_PINASSIGN5_I2C0_SCL_IO_SHIFT)|
                                    (sda<<SWM_PINASSIGN5_I2C0_SDA_IO_SHIFT));

    SYSCON->SYSAHBCLKCTRL0 &= ~(SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_I2C0_MASK );// I2C is on
    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_I2C0_RST_N_MASK );// reset I2C(bit = 0)
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_I2C0_RST_N_MASK);// remove i2C reset (bit = 1)
    SYSCON->I2C0CLKSEL = 0b000;
    I2C0->CFG = I2C_CFG_SLVEN_MASK;//(I2C_CFG_MSTEN_MASK);//I2C_CFG_SLVEN_MASK // only as primary
    I2C0->SLVADR[0] = (addr<<1) | 0b0;
    //I2C_PrimarySetBaudRate(baudRate, SystemCoreClock);
    //mkt_config();//******************************************************************
    I2C_mode = i2cModeSlave;
    NVIC_EnableIRQ(I2C0_IRQn);
    __enable_irq();
}

int WaitI2cSlaveState() 
{
    unsigned char i = 0;
    while(!(I2C0->STAT & I2C_STAT_SLVPENDING_MASK))// Wait
    {
        if (i>loopWaitSend)
            return i2cStat_TimeOut;//timeout
        i++;
    }
    return i2cStat_OK;
}
int i2cRead_slave(unsigned char buffer[])
{
    //unsigned int timeAux = SysTickGetTime_us();
    if (WaitI2cSlaveState() == i2cStat_TimeOut)
        return 0;
    I2C0->SLVCTL = CTL_SLVCONTINUE;
    for (int i = 0; i < 0xFF; i++)
    {
        if (WaitI2cSlaveState() == i2cStat_TimeOut)
            return i;
        buffer[i] = I2C0->SLVDAT;
        I2C0->SLVCTL = CTL_SLVCONTINUE;
    }
    return 0;
}
int i2cSend_slave(unsigned char buffer[], int size)
{
    if (size==0)
        return i2cStat_Fail;
    unsigned int newLen = size+2;
    unsigned char data[newLen];
    unsigned short crc = MODBUS_CRC16(buffer,size);
    memcpy(data,buffer,size);
    memcpy(&data[newLen-2], (void*)(&crc), sizeof(short));
    /*data[newLen-2] = (unsigned char)(crc);
    data[newLen-1] = (unsigned char)(crc>>8);*/
    ///////////////////////////
    unsigned int timeAux = SysTickGetTime_us();
    while(!(I2C0->STAT & I2C_STAT_SLVPENDING_MASK))// Wait
        if ((SysTickGetTime_us()-timeAux) > 1000)
            return i2cStat_TimeOut;//timeout
    /*unsigned short timeAux = 0;
    while(!(I2C0->STAT & I2C_STAT_SLVPENDING_MASK))// Wait
    {
        if (timeAux > 600)
            return i2cStat_TimeOut;//timeout
        timeAux++;
    }*/
    ////////////////////////////
    I2C0->SLVCTL = CTL_SLVCONTINUE;
    for (int i = 0; i < newLen; i++)
    {
        if (WaitI2cSlaveState()==i2cStat_TimeOut)
            return i2cStat_TimeOut;
        I2C0->SLVDAT = data[i];
        I2C0->SLVCTL = CTL_SLVCONTINUE;
    }
    return i2cStat_OK;
}
void _I2C_PrimarySetBaudRate(unsigned int baudRate_Bps, unsigned int srcClock_Hz)
{
    unsigned int scl, divider;
    unsigned int best_scl, best_div;
    unsigned int err, best_err;
    best_err = 0;
    for (scl = 9; scl >= 2; scl--)
    {
        /* calculated ideal divider value for given scl */
        divider = srcClock_Hz / (baudRate_Bps * scl * 2u);
        /* adjust it if it is out of range */
        divider = (divider > 0x10000u) ? 0x10000 : divider;
        /* calculate error */
        err = srcClock_Hz - (baudRate_Bps * scl * 2u * divider);
        if ((err < best_err) || (best_err == 0))
        {
            best_div = divider;
            best_scl = scl;
            best_err = err;
        }
        if ((err == 0) || (divider >= 0x10000u))
        {
            /* either exact value was found
            or divider is at its max (it would even greater in the next iteration for sure) */
            break;
        }
    }
    // Assign Clock Divider value, using included in LPC802.h
    I2C0->CLKDIV = I2C_CLKDIV_DIVVAL(best_div - 1);
    // Assign Primary timing configuration, using two macros include in LPC802.h
    I2C0->MSTTIME = I2C_MSTTIME_MSTSCLLOW(best_scl - 2u) | I2C_MSTTIME_MSTSCLHIGH(best_scl - 2u);
}
/*******************************************************************************
 * Master
 ******************************************************************************/
void i2cBeginMaster(unsigned char sda, unsigned char scl, unsigned int baudRate)
{
    __disable_irq(); // turn off globally
    NVIC_DisableIRQ(I2C0_IRQn);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SWM0->PINASSIGN.PINASSIGN5 &= ~(SWM_PINASSIGN5_I2C0_SCL_IO_MASK | 
                                    SWM_PINASSIGN5_I2C0_SDA_IO_MASK); // clear 15:0
    SWM0->PINASSIGN.PINASSIGN5 |= ((scl<<SWM_PINASSIGN5_I2C0_SCL_IO_SHIFT)| // Put 16 in bits 15:8
                                    (sda<<SWM_PINASSIGN5_I2C0_SDA_IO_SHIFT));
    SYSCON->SYSAHBCLKCTRL0 &= ~(SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_I2C0_MASK );// I2C is on
    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL0_I2C0_RST_N_MASK );// reset I2C(bit = 0)
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL0_I2C0_RST_N_MASK);// remove i2C reset (bit = 1)
    SYSCON->I2C0CLKSEL = 0b000;
    I2C0->CFG = (I2C_CFG_MSTEN_MASK);//I2C_CFG_SLVEN_MASK // only as primary
    _I2C_PrimarySetBaudRate(baudRate, SystemCoreClock);
    //mkt_config();//******************************************************************
    //NVIC_EnableIRQ(I2C0_IRQn);
    I2C_mode = i2cModeMaster;
    __enable_irq();
}
int WaitI2cMasterState(I2C_Type * ptr_LPC_I2C, unsigned int state) 
{
    unsigned int i = 0;
    while(!(ptr_LPC_I2C->STAT & I2C_STAT_MSTPENDING_MASK))// Wait
    {
        if (i>loopWaitReceive)
        {
            I2C0->MSTDAT = 0x00;
            I2C0->MSTCTL = MSTCTL_STOP;
            return i2cStat_TimeOut;//timeout
        }
        i++;
    }
    i=0;
    while((ptr_LPC_I2C->STAT & PRIMARY_STATE_MASK) != state)// Wait
    {
        if (i>=loopWaitReceive)
        {
            I2C0->MSTDAT = 0x00;
            I2C0->MSTCTL = MSTCTL_STOP;
            return i2cStat_TimeOut;//timeout
        }
        i++;
    }
    return i2cStat_OK; // ok
}
int WaitI2cMasterRXRDY(I2C_Type * ptr_LPC_I2C) 
{
    unsigned int i = 0;
    while((ptr_LPC_I2C->STAT & PRIMARY_STATE_MASK) != I2C_STAT_MSTST_RXRDY)// Wait
    {
        if (i>loopWaitReceive)
        {
            I2C0->MSTCTL = MSTCTL_STOP;
            WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
            return i2cStat_TimeOut;//timeout
        }
        i++;
    }
    return i2cStat_OK; // ok
}
int i2cReadLenght_master(unsigned char addr, unsigned char buffer[], int len)
{
    int i2c_stats = WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    if (i2c_stats!=i2cStat_OK)
        return i2c_stats;
    I2C0->MSTDAT = (addr<<1)|1;//send addr
    I2C0->MSTCTL = MSTCTL_START;
    unsigned int i = 0;
    while(!(I2C0->STAT & I2C_STAT_MSTPENDING_MASK))// Wait
    {
        if (i>(100*loopWaitReceive))
        {
            I2C0->MSTDAT = 0x00;
            I2C0->MSTCTL = MSTCTL_STOP;
            return i2cStat_TimeOut;//timeout
        }
        i++;
    }
    for (unsigned char i = 0; i < len; i++)
    {
        i2c_stats = WaitI2cMasterRXRDY(I2C0);
        if (i2c_stats!=i2cStat_OK)
            return i2c_stats;
        buffer[i] = I2C0->MSTDAT;
        I2C0->MSTCTL = MSTCTL_CONTINUE;
    }
    WaitI2cMasterRXRDY(I2C0);
    I2C0->MSTDAT = 0;
    I2C0->MSTCTL = MSTCTL_STOP;
    WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    return i2cStat_OK;
}
int i2cRead_master(unsigned char addr, unsigned char buffer[])
{
    int i2c_stats = WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    if (i2c_stats!=i2cStat_OK)
        return i2c_stats;
    I2C0->MSTDAT = (addr<<1)|1;//send addr
    I2C0->MSTCTL = MSTCTL_START;
    for (int i = 0; i < 0x1FF; i++)
    {
        i2c_stats = WaitI2cMasterState(I2C0, I2C_STAT_MSTST_RXRDY);
        if (i2c_stats!=i2cStat_OK)
            return i2c_stats;
        buffer[i] = I2C0->MSTDAT;
        I2C0->MSTCTL = MSTCTL_CONTINUE;
    }
    I2C0->MSTCTL = MSTCTL_STOP;
    WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    return i2cStat_OK;
}
int i2cSend_master(unsigned char addr, unsigned char *message, int size)
{
    int i2c_stats = WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    if (i2c_stats!=i2cStat_OK)
        return i2c_stats;
    unsigned int newLen = size+2;
    unsigned char data[newLen];
    unsigned short crc = MODBUS_CRC16(message,size);
    memcpy(data,message,size);
    memcpy(&data[size], (void*)(&crc), sizeof(short));
    I2C0->MSTDAT = (addr<<1)|0;//send addr
    I2C0->MSTCTL = MSTCTL_START;
    i2c_stats = WaitI2cMasterState(I2C0, I2C_STAT_MSTST_TXRDY);
    if (i2c_stats!=i2cStat_OK)
        return i2c_stats;
    for (int i = 0; i < newLen; i++)
    {
        i2c_stats = WaitI2cMasterState(I2C0, I2C_STAT_MSTST_TXRDY);
        if (i2c_stats!=i2cStat_OK)
            return i2c_stats;
        I2C0->MSTDAT = data[i];
        I2C0->MSTCTL = MSTCTL_CONTINUE;  
    }
    WaitI2cMasterState(I2C0, I2C_STAT_MSTST_TXRDY);
    I2C0->MSTDAT = 0x00;
    I2C0->MSTCTL = MSTCTL_STOP;
    WaitI2cMasterState(I2C0,I2C_STAT_MSTST_IDLE);
    return i2cStat_OK;
}



