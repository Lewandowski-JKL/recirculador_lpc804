/*
 * commomLib.cpp
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */
#include "commonLib.h"
#include "board.h"
#include <stdlib.h>

#define __SHRT_MIN__    (0xFFFF-__SHRT_MAX__)
#define __INT_MIN__     (0xFFFFFFFF-__INT_MAX__)


#ifdef MYREGISTERS_H
    myBoolRegisters BoolRegisters[regMapTotalBool];
    myShortRegisters ShortRegisters[regMapTotalShort];
    myIntRegisters IntRegisters[regMapTotalInt];
    myFloatRegisters FloatRegisters[regMapTotalFloat];

    void initRegisters()
    {
        unsigned short j = 0;
        initBoolReg(&BoolRegisters[0], true, regMapBomba);
        initBoolReg(&BoolRegisters[1], false, regMapBotoeira);
        initBoolReg(&BoolRegisters[2], true, regMapSolenoideA);
        initBoolReg(&BoolRegisters[3], false, regMapSolenoideB);
        initShortReg(&ShortRegisters[0], CODE_RECIRCULADOR, __SHRT_MAX__, __SHRT_MIN__, regMapModelo);
        j = 1;
        for (int i = 0; i < (regMapFirmwareVersionEnd - regMapFirmwareVersionInit + 1); i++, j++)
            initShortReg(&ShortRegisters[j], FirmwareVersion[i], __SHRT_MAX__, __SHRT_MIN__, regMapFirmwareVersionInit+i);
        for (int i = 0;i < (regMapEnderecoMacEnd - regMapEnderecoMacInit + 1); i++, j++)
            initShortReg(&ShortRegisters[j], __SHRT_MAX__, __SHRT_MAX__, __SHRT_MIN__, regMapEnderecoMacInit+i);
        for (int i = 0;i < (regMapNomeDaRedeEnd - regMapNomeDaRedeInit + 1); i++, j++)
            initShortReg(&ShortRegisters[j], __SHRT_MAX__, __SHRT_MAX__, __SHRT_MIN__, regMapNomeDaRedeInit+i);
        for (int i = 0;i < (regMapSenhaDaRedeEnd - regMapSenhaDaRedeInit + 1); i++, j++)
            initShortReg(&ShortRegisters[j], __SHRT_MAX__, __SHRT_MAX__, __SHRT_MIN__, regMapSenhaDaRedeInit+i);
        initIntReg(&IntRegisters[0], 0x00, __INT_MAX__, __INT_MIN__, regMapTimestamp);
        j = 1;
        for (int i = 0; i < (regMapAgendamentosEnd- regMapAgendamentosInit + 1); i++, j++)
            initIntReg(&IntRegisters[j], __INT_MIN__, __INT_MAX__, __INT_MIN__, regMapAgendamentosInit+i);
        initFloatReg(&FloatRegisters[0],  0.0, 1000.0, -1000.0, regMapFluxo);
        initFloatReg(&FloatRegisters[1], 0.0, __FLT_MAX__, 0.0, regMapLitrosTotais);
        initFloatReg(&FloatRegisters[2], 25.0, 1000.0, -1000.0, regMapTemperaturaMedida);
        initFloatReg(&FloatRegisters[3], 40.0, 60.0, 20.0, regMapTemperaturaRef);
        initFloatReg(&FloatRegisters[4], 1.0, 2.0, 0.5, regMapCalibFluxo);
    }

    void _getBool(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        int j = 0;
        for (int i = 0; i < regMapTotalBool; i++)
        {
            if (BoolRegisters[i]._addr == firstRegisterAddr)
            {
                if ((i+registerQuantity) > regMapTotalBool)
                    return;
                j=i;
                break;
            }
        }
        char totalReg = registerQuantity/8;
        if (registerQuantity%8)
            totalReg++;
        bool _boolVet[registerQuantity];
        for (int i = 0; i < registerQuantity; i++)
#ifdef __LOW_MEMORY__
            _boolVet[i] = getBoolRegister(&BoolRegisters[i+j]);
#else
            _boolVet[i] = BoolRegisters[i+j].get(&BoolRegisters[i+j]);
#endif
        bufferTX[0] = GET_NUMERIC_REGISTERS;
        bufferTX[1] = totalReg + 2;//+2 é o CRC
        *sizeTX = copyBoolToPChar(_boolVet, registerQuantity, &bufferTX[2])+2;
    }
    void _getShort(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        int j = 0;
        for (int i = 0; i < regMapTotalShort; i++)
        {
            if (ShortRegisters[i]._addr == firstRegisterAddr)
            {
                if ((i+registerQuantity) > regMapTotalShort)
                    return;
                j=i;
                break;
            }
        }
        short _shortVet[registerQuantity];
        for (int i = 0; i < registerQuantity; i++)
#ifdef __LOW_MEMORY__
            _shortVet[i] = getShortRegister(&ShortRegisters[i+j]);
#else
            _shortVet[i] = ShortRegisters[i+j].get(&ShortRegisters[i+j]);
#endif
        bufferTX[0] = GET_NUMERIC_REGISTERS;
        bufferTX[1] = sizeof(short)*registerQuantity + 2;//+2 é o CRC
        *sizeTX = bufferTX[1];
        memcpy(&bufferTX[2], _shortVet, registerQuantity);
        //*sizeTX = copyShortToPChar(_shortVet, registerQuantity, &bufferTX[2])+2;
    }
    void _getInt(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        int j = 0;
        for (int i = 0; i < regMapTotalInt; i++)
        {
            if (IntRegisters[i]._addr == firstRegisterAddr)
            {
                if ((i+registerQuantity) > regMapTotalInt)
                    return;
                j=i;
                break;
            }
        }
        int _intVet[registerQuantity];
        for (int i = 0; i < registerQuantity; i++)
#ifdef __LOW_MEMORY__
            _intVet[i] = getIntRegister(&IntRegisters[i+j]);
#else
            _intVet[i] = IntRegisters[i+j].get(&IntRegisters[i+j]);
#endif
        bufferTX[0] = GET_NUMERIC_REGISTERS;
        bufferTX[1] = sizeof(int)*registerQuantity + 2;//+2 é o CRC
        //*sizeTX = copyIntToPChar(_intVet, registerQuantity, &bufferTX[2])+2;
        *sizeTX = bufferTX[1];
        memcpy(&bufferTX[2], _intVet, registerQuantity);
    }
    void _getFloat(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        int j = 0;
        for (int i = 0; i < regMapTotalFloat; i++)
        {
            if (FloatRegisters[i]._addr == firstRegisterAddr)
            {
                if ((i+registerQuantity) > regMapTotalFloat)
                    return;
                j=i;
                break;
            }
        }
        float _floatVet[registerQuantity];
        for (int i = 0; i < registerQuantity; i++)
#ifdef __LOW_MEMORY__
            _floatVet[i] = getFloatRegister(&FloatRegisters[i+j]);
#else
            _floatVet[i] = FloatRegisters[i+j].get(&FloatRegisters[i+j]);
#endif
        bufferTX[0] = GET_NUMERIC_REGISTERS;
        bufferTX[1] = sizeof(float)*registerQuantity + 2;//+2 é o CRC
        *sizeTX = copyFloatToPChar(_floatVet, registerQuantity, &bufferTX[2])+2;
        return;
        *sizeTX = bufferTX[1];
        /*for (unsigned char i = 0; i < bufferTX[1]; i++)
            bufferTX[2+i] = i;
        return;*/
        memcpy(&bufferTX[2], _floatVet, registerQuantity);
    }

    void getBooleanRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    { 
        if (*sizeRX != 7)//7 por conta do crc
            return;
        short firstRegisterAddr = ((short)bufferRX[2]<<8) + bufferRX[1];
        short registerQuantity = ((short)bufferRX[4]<<8) + bufferRX[3];
        int j = 0;
        for (int i = 0; i < regMapTotalBool; i++)
        {
            if (BoolRegisters[i]._addr == firstRegisterAddr)
            {
                if ((i+registerQuantity) > regMapTotalBool)
                    return;
                j=i;
                break;
            }
        }
        char totalReg = registerQuantity/8;//numero de registradores booleanos por byte da mensagem
        if (registerQuantity%8)
            totalReg++;
        bool _boolVet[registerQuantity];
        for (int i = 0; i < registerQuantity; i++)
#ifdef __LOW_MEMORY__
            _boolVet[i] = getBoolRegister(&BoolRegisters[i+j]);
#else
            _boolVet[i] = BoolRegisters[i+j].get(&BoolRegisters[i+j]);
#endif
        bufferTX[0] = GET_NUMERIC_REGISTERS;
        bufferTX[1] = totalReg + 2;//+2 é o CRC
        *sizeTX = copyBoolToPChar(_boolVet, registerQuantity, &bufferTX[2])+2;
    }
    void getNumericRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        short firstRegisterAddr = 0;
        //memcpy(&firstRegisterAddr,&bufferRX[1],sizeof(short));
        copyPCharToShort(&bufferRX[1],2,&firstRegisterAddr);
        short registerQuantity = 0;
        copyPCharToShort(&bufferRX[3],2,&registerQuantity);
        //memcpy(&firstRegisterAddr,&bufferRX[3],sizeof(short));
        switch (firstRegisterAddr)
        {
        case 0x03E9 ... 0x07CF://boolean register
            _getBool(firstRegisterAddr, registerQuantity, bufferTX, sizeTX);
            break;
        case 0x0BB9 ... 0x0F9F://short register
            _getShort(firstRegisterAddr, registerQuantity, bufferTX, sizeTX);
            break;
        case 0x1389 ... 0x176F://int register
            _getInt(firstRegisterAddr, registerQuantity, bufferTX, sizeTX);
            break;
        case 0x1B59 ... 0x1F3F://float register
            _getFloat(firstRegisterAddr, registerQuantity, bufferTX, sizeTX);
            break;
        default:
            break;
        }
    }
    void setBooleanRegister(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        
    }
    void setNumericRegister(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        
    }
    void setBooleanRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        
    }
    void setNumericRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {

    }

    void processMessage(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
    {
        if (!CHECK_CRC16(bufferRX,*sizeRX))
        {
            *sizeRX = 0;
            return;
        }   
        *sizeTX = 0;
        switch (bufferRX[0])
        {
        case GET_BOOLEAN_REGISTERS:
            getBooleanRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        case GET_NUMERIC_REGISTERS:
            getNumericRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        case SET_BOOLEAN_REGISTER:
            setBooleanRegister(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        case SET_SINGLE_REGISTER:
            setNumericRegister(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        case SET_MULTIPLE_BOOLEAN_REGISTERS:
            setBooleanRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        case SET_MULTIPLE_REGISTERS:
            setNumericRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
            break;
        default:
            break;
        }
    }


    void setBoolWithAddr(bool value, short addr)
    {
        for (int i = 0; i < regMapTotalBool; i++)
            if (BoolRegisters[i]._addr == addr)
            {
                BoolRegisters[i]._value = value;
                return;
            }
    }
    bool getBoolWithAddr(short addr)
    {
        for (int i = 0; i < regMapTotalBool; i++)
            if (BoolRegisters[i]._addr == addr)
                return BoolRegisters[i]._value;
        return false;
    }
    void setShortWithAddr(short value, short addr)
    {
        for (int i = 0; i < regMapTotalShort; i++)
            if (ShortRegisters[i]._addr == addr)
            {
                ShortRegisters[i]._value = value;
                return;
            }
    }
    short getShortWithAddr(short addr)
    {
        for (int i = 0; i < regMapTotalShort; i++)
            if (ShortRegisters[i]._addr == addr)
                return ShortRegisters[i]._value;
        return 0;
    }
    void setIntWithAddr(int value, short addr)
    {
        for (int i = 0; i < regMapTotalInt; i++)
            if (IntRegisters[i]._addr == addr)
            {
                IntRegisters[i]._value = value;
                return;
            }
    }
    int getIntWithAddr(short addr)
    {
        for (int i = 0; i < regMapTotalInt; i++)
            if (IntRegisters[i]._addr == addr)
                return IntRegisters[i]._value;
        return 0;
    }
    void setFloatWithAddr(float value, short addr)
    {
        for (int i = 0; i < regMapTotalFloat; i++)
            if (FloatRegisters[i]._addr == addr)
            {
                FloatRegisters[i]._value = value;
                return;
            }
    }
    float getFloatWithAddr(short addr)
    {
        for (int i = 0; i < regMapTotalFloat; i++)
            if (FloatRegisters[i]._addr == addr)
                return FloatRegisters[i]._value;
        return 0.0;
    }

#endif

const short FirmwareVersion[3] = {0,1,0};

//Extern variables
//extern SMHandle *SerialSend;
//extern SMHandle *SerialReceive;
unsigned char TaskRunFlag;
unsigned int DebugTaskFlag;

#ifdef MYINTERRUPT_H
void delayMs(unsigned int delay)
{
    /*delay *= 666;///1.5us por loop ~ considerando 30Mhz
    for (unsigned i = 0; i < delay; i++);*/
    unsigned int time = SysTickGetTime_us();
    while (((SysTickGetTime_us()-time)<=(delay*1000)));
}
void delay_us(unsigned int delay)
{
    delay = ((delay-10)*10)/15;
    for (unsigned i = 0; i < delay; i++);
    /*unsigned int time = SysTickGetTime_us();
    while (((SysTickGetTime_us()-time)<=delay));*/
}
#endif

void setClock30M()
{
    
    asm volatile(                      // the void set_fro_frequency(uint32_t iFreq); API is missing so do this by hand
    " ldr R1, ROM_Tbl \n"    // get address of pointer to ROM Driver Table
    " ldr R1, [R1] \n"             // get pointer to ROM Driver Table
    " add R1, #0x0C \n"       // step down to the entry for the FRO table
    " ldr R1, [R1] \n"             // get address of FRO table
    " add R1, #8 \n"             // step down to third entry in table
    " ldr R1, [R1] \n"             // get address of set_fro_frequency routine
    " ldr R0, FRO_Freq30M \n"    // put frequency in R0
    " blx R1 \n"                      // call API
    " b Exit \n"                      // done

    " .align 4 \n"                   // fix alignment
    "FRO_Freq30M: .word 30000 \n" // frequency in kHz
    "ROM_Tbl: .word 0x0F001FF8 \n" // address of ROM Driver Table

    "Exit: \n"                         // end of code segment
    );
    SystemCoreClock = 15000000;
    /*
    // ----------------------- Begin Core Clock Select -----------------------
    // Specify that we will use the Free-Running Oscillator
    // Set the main clock to be the FRO
    // 0x0 is FRO; 0x1 is external clock ; 0x2 is Low pwr osc.; 0x3 is FRO_DIV
    // Place in bits 1:0 of MAINCLKSEL.
    SYSCON->MAINCLKSEL = (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    // Update the Main Clock
    // Step 1. write 0 to bit 0 of this register
    // Step 2. write 1 to bit 0 this register
    SYSCON->MAINCLKUEN &= ~(0x1); // step 1. (Sec. 6.6.4 of manual)
    SYSCON->MAINCLKUEN |= 0x1; // step 2. (Sec. 6.6.4 of manual)

    BOARD_BootClockFRO30M();*/
}
void setClock24M()
{
    asm volatile(                      // the void set_fro_frequency(uint32_t iFreq); API is missing so do this by hand
    " ldr R1, ROM_Tbl24M \n"    // get address of pointer to ROM Driver Table
    " ldr R1, [R1] \n"             // get pointer to ROM Driver Table
    " add R1, #0x0C \n"       // step down to the entry for the FRO table
    " ldr R1, [R1] \n"             // get address of FRO table
    " add R1, #8 \n"             // step down to third entry in table
    " ldr R1, [R1] \n"             // get address of set_fro_frequency routine
    " ldr R0, FRO_Freq24M \n"    // put frequency in R0
    " blx R1 \n"                      // call API
    " b Exit24M \n"                      // done

    " .align 4 \n"                   // fix alignment
    "FRO_Freq24M: .word 24000 \n" // frequency in kHz
    "ROM_Tbl24M: .word 0x0F001FF8 \n" // address of ROM Driver Table

    "Exit24M: \n"                         // end of code segment
    );
    SystemCoreClock = 12000000;
    /*SYSCON->MAINCLKSEL = (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    SYSCON->MAINCLKUEN &= ~(0x1);
    SYSCON->MAINCLKUEN |= 0x1; 
    BOARD_BootClockFRO24M();*/
}
void setClock18M()
{
    asm volatile(                      // the void set_fro_frequency(uint32_t iFreq); API is missing so do this by hand
    " ldr R1, ROM_Tbl18M \n"    // get address of pointer to ROM Driver Table
    " ldr R1, [R1] \n"             // get pointer to ROM Driver Table
    " add R1, #0x0C \n"       // step down to the entry for the FRO table
    " ldr R1, [R1] \n"             // get address of FRO table
    " add R1, #8 \n"             // step down to third entry in table
    " ldr R1, [R1] \n"             // get address of set_fro_frequency routine
    " ldr R0, FRO_Freq18M \n"    // put frequency in R0
    " blx R1 \n"                      // call API
    " b Exit18M \n"                      // done

    " .align 4 \n"                   // fix alignment
    "FRO_Freq18M: .word 18000 \n" // frequency in kHz
    "ROM_Tbl18M: .word 0x0F001FF8 \n" // address of ROM Driver Table

    "Exit18M: \n"                         // end of code segment
    );
    SystemCoreClock = 9000000;
    /*SYSCON->MAINCLKSEL = (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    SYSCON->MAINCLKUEN &= ~(0x1);
    SYSCON->MAINCLKUEN |= 0x1; 
    BOARD_BootClockFRO18M();*/
}

short copyBoolToPChar(bool *src, short size, unsigned char *dest)
{
    unsigned short dim = size/8;
    if (size%8)
        dim++;
    for (int i = 0; i < (dim*8); i++)
    {
        if (i>=size)
            dest[i/8] = (dest[i/8] << 1);
            else
                dest[i/8] = (dest[i/8] << 1) + src[i];
    }
    return dim;
}
short copyShortToPChar(short *src, short size, unsigned char *dest)
{
    for (int i = 0; i < (sizeof(short)*size); i++)
        dest[i]=(unsigned char)src[i/sizeof(short)] >> (8*(i%sizeof(short)));
    return sizeof(short)*size;
}
short copyIntToPChar(int *src, short size, unsigned char *dest)
{
    for (int i = 0; i < (sizeof(int)*size); i++)
        dest[i]=(unsigned char)src[i/sizeof(int)] >> (8*(i%sizeof(int)));
    return sizeof(int)*size;
}
short copyFloatToPChar(float *src, short size, unsigned char *dest)
{
    for (int i = 0; i < (sizeof(float)*size); i++)
        dest[i]=(unsigned char)src[i/sizeof(float)] >> (8*(i%sizeof(float)));
    return sizeof(float)*size;
}

short copyPCharToBool(unsigned char *src, short size, bool *dest)
{
    return 0;
}
short copyPCharToShort(unsigned char *src, short size, short *dest)
{
    short dim = size/sizeof(short);
    if (size%sizeof(short))
        dim++;
    short valueAux= 0;
    for (int i = 0; i < (dim*sizeof(short)); i++)
    {
        if (i >= size)
            valueAux = 0;
            else
                valueAux = src[i];
        dest[i/sizeof(short)] += valueAux << (8*(i%sizeof(short)));
    }
    return dim;
}
short copyPCharToInt(unsigned char *src, short size, int *dest)
{
    short dim = size/sizeof(int);
    if (size%sizeof(int))
        dim++;
    int valueAux= 0;
    for (int i = 0; i < (dim*sizeof(int)); i++)
    {
        if (i >= size)
            valueAux = 0;
            else
                valueAux = src[i];
        dest[i/sizeof(int)] += valueAux << (8*(i%sizeof(int)));
    }
    return dim;
}
short copyPCharToFloat(unsigned char *src, short size, float *dest)
{
    short dim = size/sizeof(float);
    if (size%sizeof(float))
        dim++;
    int valueAux = 0;
    for (int i = 0; i < (dim*sizeof(float)); i++)
    {
        if (i >= size)
            valueAux = 0;
            else
                valueAux = src[i];
        dest[i/sizeof(float)] += valueAux << (8*(i%sizeof(float)));
    }
    return dim;
}
