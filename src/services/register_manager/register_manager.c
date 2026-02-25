#include <stdlib.h>
#include "register_manager.h"
#include "board.h"
#include "app_config.h"

#define __SHRT_MIN__    (0xFFFF-__SHRT_MAX__)
#define __INT_MIN__     (0xFFFFFFFF-__INT_MAX__)

volatile bool LastBoolRegisters[regMapTotalBool];
volatile short LastShortRegisters[regMapTotalShort];
volatile int LastIntRegisters[regMapTotalInt];
volatile short LastFloatRegisters[regMapTotalFloat];

volatile bool BoolRegisters[regMapTotalBool];
volatile short ShortRegisters[regMapTotalShort];
volatile int IntRegisters[regMapTotalInt];
volatile short FloatRegisters[regMapTotalFloat];

void initRegisters()
{
    unsigned short j = 0;
    BoolRegisters[0] = true;
    BoolRegisters[1] = false;
    BoolRegisters[2] = true;
    BoolRegisters[3]= false;
    ShortRegisters[0] = CODE_RECIRCULADOR;
    j = 1;
    for (int i = 0; i < (regMapFirmwareVersionEnd - regMapFirmwareVersionInit + 1); i++, j++)
        ShortRegisters[j] = FirmwareVersion[i];
    for (int i = 0;i < (regMapEnderecoMacEnd - regMapEnderecoMacInit + 1); i++, j++)
        ShortRegisters[j] = 0xFFFF;
    for (int i = 0;i < (regMapNomeDaRedeEnd - regMapNomeDaRedeInit + 1); i++, j++)
        ShortRegisters[j] = 0xFFFF;
    for (int i = 0;i < (regMapSenhaDaRedeEnd - regMapSenhaDaRedeInit + 1); i++, j++)
        ShortRegisters[j] = 0xFFFF;
    IntRegisters[0] = 0x00;//1712333400;//0x00;
    j = 1;
    //define todos os agendamentos de segunda a sexta das 5-19hrs
    for (int i = 0; i < (regMapAgendamentosEnd- regMapAgendamentosInit + 1); i++, j++)
        IntRegisters[j] = 0b11111111100100101100110001110100;
    FloatRegisters[0] = 0;
    FloatRegisters[1] = 2500;
    FloatRegisters[2] = 0;
    FloatRegisters[3] = 4000;
    FloatRegisters[4] = 100;
    FloatRegisters[5] = 100;
    /*
    regMapFluxo = regMapFloatOffset+1,
    regMapTemperaturaMedida,
    regMapLitrosTotais,
    regMapTemperaturaRef,
    regMapCalibFluxo,
    regMapCalibTemp,
    */
}

void _getBool(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX)
{
    short newAddr = (firstRegisterAddr-regMapBoolOffset-1);
    if ((newAddr + registerQuantity) > regMapTotalBool)
        return;
    char totalReg = registerQuantity/8;
    if (registerQuantity%8)
        totalReg++;
    bool _boolVet[registerQuantity];
    memcpy(_boolVet, (void*)(BoolRegisters+newAddr), (registerQuantity*typeBool));
    bufferTX[0] = GET_NUMERIC_REGISTERS;
    bufferTX[1] = totalReg + 2;//+2 é o CRC
    *sizeTX = copyBoolToPChar(_boolVet, registerQuantity, &bufferTX[2])+2;
}
void _getValue(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX, unsigned char *sizeTX,
                void *ptrReg, short registerTypeOffset, short registersTotal, char size)
{
    short newAddr = (firstRegisterAddr-registerTypeOffset-1);
    if ((newAddr + registerQuantity) > registersTotal)
        return;
    bufferTX[0] = GET_NUMERIC_REGISTERS;
    bufferTX[1] = size*registerQuantity + 2;//+2 é o CRC
    *sizeTX = bufferTX[1];
    memcpy(&bufferTX[2], (void*)(ptrReg+(newAddr*size)), (registerQuantity*size));
}
void getNumericRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
{
    *sizeTX = 0;
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
        _getValue(firstRegisterAddr, registerQuantity,bufferTX,sizeTX,
                    (void*)ShortRegisters, regMapShortOffset, regMapTotalShort, typeShort);
        break;
    case 0x1389 ... 0x176F://int register
        _getValue(firstRegisterAddr, registerQuantity, bufferTX, sizeTX,
                    (void*)IntRegisters, regMapIntOffset, regMapTotalInt, typeInt);
        break;
    case 0x1B59 ... 0x1F3F://float register
        _getValue(firstRegisterAddr, registerQuantity,bufferTX,sizeTX,
                    (void*)FloatRegisters, regMapFloatOffset, regMapTotalFloat, typeShort);//estou usando tipo short
        break;
    default:
        break;
    }
}
unsigned char _setBool(short firstRegisterAddr, short registerQuantity, unsigned char *bufferTX)
{
    return 0;
}
unsigned char _setValue(short firstRegisterAddr, short registerQuantity, unsigned char *bufferRX,
                void *ptrReg, short registerTypeOffset, short registersTotal, char size)
{
    if (bufferRX[0]!=(registerQuantity*size))
        return 0;
    short newAddr = (firstRegisterAddr-registerTypeOffset-1);
    if ((newAddr + registerQuantity) > registersTotal)
        return 0;
    memcpy((void*)(ptrReg+(newAddr*size)), &bufferRX[1], (registerQuantity*size));
    return 1;
}
void setNumericRegisters(unsigned char *bufferRX, unsigned char *sizeRX, unsigned char *bufferTX, unsigned char *sizeTX)
{
    
    *sizeTX = 0;
    short firstRegisterAddr = 0;
    copyPCharToShort(&bufferRX[1],2,&firstRegisterAddr);
    short registerQuantity = 0;
    copyPCharToShort(&bufferRX[3],2,&registerQuantity);
    switch (firstRegisterAddr)
    {
    case 0x03E9 ... 0x07CF://boolean register
        *sizeTX = _setBool(firstRegisterAddr,registerQuantity, &bufferRX[5]);
        break;
    case 0x0BB9 ... 0x0F9F://short register
        *sizeTX = _setValue(firstRegisterAddr, registerQuantity, &bufferRX[5],
                    (void*)ShortRegisters, regMapShortOffset, regMapTotalShort, typeShort);
        break;
    case 0x1389 ... 0x176F://int register
        *sizeTX = _setValue(firstRegisterAddr, registerQuantity, &bufferRX[5],
                    (void*)IntRegisters, regMapIntOffset, regMapTotalInt, typeInt);
        break;
    case 0x1B59 ... 0x1F3F://float register
        *sizeTX = _setValue(firstRegisterAddr, registerQuantity, &bufferRX[5],
                    (void*)FloatRegisters, regMapFloatOffset, regMapTotalFloat, typeShort);//estou usando tipo short
        break;
    default:
        break;
    }
    if (*sizeTX)
    {
        memcpy(bufferTX,bufferRX, *sizeRX-2);
        *sizeTX = *sizeRX-2;
    }
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
    case GET_NUMERIC_REGISTERS:
        getNumericRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
        break;
    case SET_MULTIPLE_REGISTERS:
        /*memcpy(bufferTX,bufferRX, *sizeRX-2);
        *sizeTX = *sizeRX-2;*/
        setNumericRegisters(bufferRX, sizeRX, bufferTX, sizeTX);
        break;
    default:
        break;
    }
}

void setValueWithAddr(void *value, void *ptr, short addr, char size)
{
    addr--;
    memcpy((void*)(ptr + size*addr), value,size);
}
int getValueWithAddr(void *ptr, short addr, char size)
{
    addr--;
    int value;
    memcpy(&value, (void*)(ptr + size*addr), size);
    return value;
}
bool getBoolWithAddr(short addr)
{
    addr -= regMapBoolOffset;
    addr--;
    return BoolRegisters[addr];
}
void setBoolValue(bool value, short addr)
{
    setValueWithAddr(&value, (void*)BoolRegisters, addr-regMapBoolOffset, typeBool);
}
short getShortWithAddr(short addr)
{
    return (short)getValueWithAddr((void*)ShortRegisters,(addr-regMapShortOffset),typeShort);
}
void setShortValue(short value, short addr)
{
    setValueWithAddr(&value, (void*)ShortRegisters, addr-regMapShortOffset, typeShort);
}
int getIntWithAddr(short addr)
{
    return getValueWithAddr((void*)IntRegisters,(addr-regMapIntOffset),typeInt);
}
void setIntValue(int value, short addr)
{
    setValueWithAddr(&value, (void*)IntRegisters, addr-regMapIntOffset, typeInt);
}
short getFloatWithAddr(short addr)
{
    return (short)getValueWithAddr((void*)FloatRegisters,(addr-regMapFloatOffset),typeShort);//estou usando tipo short
}
void setFloatValue(int value, short addr)
{
    setValueWithAddr(&value, (void*)FloatRegisters, addr-regMapFloatOffset, typeShort);//estou usando tipo short
}

int compareBoolRegister(unsigned char *buffer)
{
    bool compare = false;
    for (int i = 0; i < sizeof(LastBoolRegisters); i++)
    {
        if (LastBoolRegisters[i] != BoolRegisters[i])
            compare = true;
        LastBoolRegisters[i] = BoolRegisters[i];
    }
    if (!compare)
        return 0;
    unsigned char sizeTX=0;
    unsigned char bufferAux[10];
    _getBool(regMapBomba, regMapTotalBool, bufferAux, &sizeTX);
    if (!sizeTX)
        return 0;
    short addrAux = regMapBomba;
    short regQuanAux = regMapTotalBool;
    buffer[0] = SET_MULTIPLE_REGISTERS;
    memcpy(&buffer[1], &addrAux, sizeof(short));
    memcpy(&buffer[3], &regQuanAux, sizeof(short));
    buffer[5] = sizeTX-2;
    memcpy(&buffer[6], &bufferAux[2], sizeTX-2);
    return sizeTX+4;
}

int compareReg(unsigned char *buffer, unsigned short addr, unsigned short numReg,
                void *regRef, void *compareReg, unsigned short size)
{
    bool compare = false;
    for (int i = 0; i < size; i++)
    {
        if ((*(unsigned char*)(regRef + i)) != (*(unsigned char*)(compareReg + i)))
        {
            compare = true;
            break;
        }
    }
    //compare = true;
    if (compare)
    {
        buffer[0] = SET_MULTIPLE_REGISTERS;
        memcpy(&buffer[1], &addr, sizeof(short));
        memcpy(&buffer[3], &numReg, sizeof(short));
        buffer[5] = size;
        memcpy(&buffer[6], regRef, size);
        return size+6;
    }
    return 0;
}
int _compareReg(unsigned char *buffer, unsigned short addr, unsigned short numReg,
                void *regRef, void *lastReg, unsigned short size)
{
    bool compare = false;
    for (int i = 0; i < size; i++)
    {
        if ((*(unsigned char*)(regRef + i)) != (*(unsigned char*)(lastReg + i)))
        {
            compare = true;
            (*(unsigned char*)(lastReg + i)) = (*(unsigned char*)(regRef + i));
        }
    }
    compare = true;////////////debug
    if (compare)
    {
        buffer[0] = SET_MULTIPLE_REGISTERS;
        memcpy(&buffer[1], &addr, sizeof(short));
        memcpy(&buffer[3], &numReg, sizeof(short));
        buffer[5] = size;
        memcpy(&buffer[6], regRef, size);
        return size+6;
    }
    return 0;
}
int compareShortRegister(unsigned char *buffer)
{
    return _compareReg(buffer, regMapShortOffset+1, regMapTotalShort, 
                    (void*)ShortRegisters, (void*)LastShortRegisters, sizeof(ShortRegisters));
}
int compareIntRegister(unsigned char *buffer)
{
    return _compareReg(buffer, regMapIntOffset+1, regMapTotalInt, 
                    (void*)IntRegisters, (void*)LastIntRegisters, sizeof(IntRegisters));
}
int compareFloatRegister(unsigned char *buffer)
{
    return _compareReg(buffer, regMapFloatOffset+1, regMapTotalFloat, 
                    (void*)FloatRegisters, (void*)LastFloatRegisters, sizeof(FloatRegisters));
}

