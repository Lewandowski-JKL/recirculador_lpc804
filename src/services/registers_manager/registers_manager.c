#include <stdlib.h>
#include "registers_manager.h"
#include "board.h"
#include "app_config.h"
#include "flash_map.h"
#include "flash_memory.h"


#define __SHRT_MIN__    (0xFFFF-__SHRT_MAX__)
#define __INT_MIN__     (0xFFFFFFFF-__INT_MAX__)

typedef struct registers_manager
{
    uint8_t _bool_reg_vet[Sys_RegMap_Nreg_Bool_tot/8];
    short _short_reg_vet[Sys_RegMap_Nreg_Short_tot];
    int _int_reg_vet[Sys_RegMap_Nreg_Int_tot];
#ifndef __NO_FLOAT__
    float _float_reg_vet[Sys_RegMap_Nreg_Float_tot];
#endif
}registers_manager;

registers_manager reg_vet;

char change_flag = 0;

/*************************** */
bool reg_eeprom_test(unsigned int addr)
{
    return false;
}
/************************** */
void reg_Begin()
{
    iap_Begin();
    iap_ReadVet((unsigned char*)(reg_vet._bool_reg_vet), Sys_RegMap_Total_Bytes, 0);
    //verifica se a eeprom ja foi iniciada alguma vez
    if (reg_vet._short_reg_vet[Sys_RegMap_GetIndex(Sys_RegMap_Model)] == Sys_equip_code)
        return;
    //Caso não foi deve buscar o sistema de arquivos de backup
    memcpy((unsigned char*)(reg_vet._bool_reg_vet), (unsigned char*)&nv_defaults.data , Sys_RegMap_Total_Bytes);
    //agora deve salvar toda a memória
    iap_WriteVet((unsigned char*)(reg_vet._bool_reg_vet), Sys_RegMap_Total_Bytes, 0);
    iap_Commit();
}

/***********************************
 * Write
 ***********************************/
int _bool_index_addr(int index)
{
    return index/8;
}
int _bool_index_pos(int index)
{
    return index%8;
}
void reg_write_bool(bool value, unsigned int addr)
{
    int index_aux = Sys_RegMap_GetIndex(addr);
    if (index_aux < 0)
        return;
    char value_register = reg_vet._bool_reg_vet[_bool_index_addr(index_aux)];
    char value_write;
    if (value)
        value_write =  reg_vet._bool_reg_vet[_bool_index_addr(index_aux)] || (0b10000000 >> _bool_index_pos(index_aux));
        else
            value_write =  reg_vet._bool_reg_vet[_bool_index_addr(index_aux)] & ~(0b10000000 >> _bool_index_pos(index_aux));
    if (value_register != value_write)
    {
        reg_vet._bool_reg_vet[_bool_index_addr(index_aux)] = value_write;
        change_flag |= Reg_Change_Bool_Register;
    }
}
void reg_write_bool_vet(bool *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Bool_tot)
        return;
    for (int i = 0; i < size; i++)
        reg_write_bool(vet[i], addr+i);
}

void reg_write_short(short value, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if (reg_vet._short_reg_vet[index] != value)
    {
        reg_vet._short_reg_vet[index]=value;
        change_flag |= Reg_Change_Short_Register;
    }
}
void reg_write_short_vet(short *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Short_tot)
        return;
    for (int i = 0; i < size; i++)
    {
        if (vet[i] != reg_vet._short_reg_vet[index+i])
        {
            memcpy((void*)&reg_vet._short_reg_vet[index], (void*)vet, size*sizeof(short));
            change_flag |= Reg_Change_Short_Register;
            return;
        }
    }
}
void reg_write_int(int value, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if (reg_vet._int_reg_vet[index] != value)
    {
        reg_vet._int_reg_vet[index]=value;
        change_flag |= Reg_Change_Short_Register;
    }
}
void reg_write_int_vet(int *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Int_tot)
        return;
    for (int i = 0; i < size; i++)
    {
        if (vet[i] != reg_vet._short_reg_vet[index+i])
        {
            memcpy((void*)&reg_vet._int_reg_vet[index], (void*)vet, size*sizeof(int));
            change_flag |= Reg_Change_Int_Register;
            return;
        }
    }
}

#ifndef __NO_FLOAT__
void reg_write_float(float value, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    reg_vet._float_reg_vet[index]=value;
}
void reg_write_float_vet(float *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Float_tot)
        return;
    memcpy((void*)&reg_vet._float_reg_vet[index], (void*)vet, size*sizeof(float));
}
#endif

void reg_write(void *val, unsigned int addr)
{
    switch (addr)
    {
    case Sys_RegMap_Offset_Bool ... (Sys_RegMap_Offset_Bool+Sys_RegMap_Nreg_Bool_tot):
        reg_write_bool((*(bool*)val), addr);
        break;
    case Sys_RegMap_Offset_Short ... (Sys_RegMap_Offset_Short+Sys_RegMap_Nreg_Short_tot):
        reg_write_short((*(short*)val), addr);
        break;
    case Sys_RegMap_Offset_Int ... (Sys_RegMap_Offset_Int+Sys_RegMap_Nreg_Int_tot):
        reg_write_int((*(int*)val), addr);
        break;
#ifndef __NO_FLOAT__
    case Sys_RegMap_Offset_Float... (Sys_RegMap_Offset_Float+Sys_RegMap_Nreg_Float_tot):
        reg_write_float((*(float*)val), addr);
        break;
#endif
    default:
        break;
    }
}
void reg_write_vet(void *vet, unsigned int nRegs, unsigned int addr)
{
    switch (addr)
    {
    case Sys_RegMap_Offset_Bool ... (Sys_RegMap_Offset_Bool+Sys_RegMap_Nreg_Bool_tot):
        reg_write_bool_vet((bool*)vet, nRegs, addr);
        break;
    case Sys_RegMap_Offset_Short ... (Sys_RegMap_Offset_Short+Sys_RegMap_Nreg_Short_tot):
        reg_write_short_vet((short*)vet, nRegs, addr);
        break;
    case Sys_RegMap_Offset_Int ... (Sys_RegMap_Offset_Int+Sys_RegMap_Nreg_Int_tot):
        reg_write_int_vet((int*)vet, nRegs, addr);
        break;
#ifndef __NO_FLOAT__
    case Sys_RegMap_Offset_Float... (Sys_RegMap_Offset_Float+Sys_RegMap_Nreg_Float_tot):
        reg_write_float((float*)vet, nRegs, addr);
        break;
#endif
    default:
        break;
    }
}

/***********************************
 * Read
 ***********************************/
bool reg_read_bool(unsigned int addr)
{
    int index_aux = Sys_RegMap_GetIndex(addr);
    if (index_aux < 0)
        return false;
    unsigned char value_aux = reg_vet._short_reg_vet[_bool_index_addr(index_aux)] 
                            && (0b10000000 >> _bool_index_pos(index_aux));
    return value_aux ? true : false;
}
void reg_read_bool_vet(bool *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Bool_tot)
        return;
    for (int i = 0; i < size; i++)
        vet[i] = reg_read_bool(addr+i);
}

short reg_read_short(unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    return index < 0 ? 0 : reg_vet._short_reg_vet[index];
}
void reg_read_short_vet(short *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Short_tot)
        return;
    memcpy(vet, &reg_vet._short_reg_vet[index], size*sizeof(short));
}
int reg_read_int(unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    return index < 0 ? 0 : reg_vet._int_reg_vet[index];
}
void reg_read_int_vet(int *vet, unsigned int size, unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_Int_tot)
        return;
    memcpy(vet, &reg_vet._int_reg_vet[index], size*sizeof(int));
}

#ifndef __NO_FLOAT__
float reg_read_float(unsigned int addr)
{
    int index = Sys_RegMap_GetIndex(addr);
    return index < 0 ? 0 : reg_vet._float_reg_vet[index];
}
void reg_read_float_vet(float *vet, unsigned int size, unsigned int addr)
{
    
    int index = Sys_RegMap_GetIndex(addr);
    if (index < 0)
        return;
    if ((index + size) > Sys_RegMap_Nreg_float_tot)
        return;
    memcpy(vet, &reg_vet._float_reg_vet[index], size*sizeof(float));
}
#endif

void reg_read(void *ptr, unsigned int addr)
{
    switch (addr)
    {
    case Sys_RegMap_Offset_Bool ... (Sys_RegMap_Offset_Bool+Sys_RegMap_Nreg_Bool_tot):
        *(bool*)ptr = reg_read_bool(addr);
        break;
    case Sys_RegMap_Offset_Short ... (Sys_RegMap_Offset_Short+Sys_RegMap_Nreg_Short_tot):
        *(short*)ptr = reg_read_short(addr);
        break;
    case Sys_RegMap_Offset_Int ... (Sys_RegMap_Offset_Int+Sys_RegMap_Nreg_Int_tot):
        *(int*)ptr = reg_read_int(addr);
        break;
#ifndef __NO_FLOAT__
    case Sys_RegMap_Offset_Float... (Sys_RegMap_Offset_Float+Sys_RegMap_Nreg_Float_tot):
        *(float*)ptr = reg_read_float(addr);
        break;
#endif
    default:
        ptr = NULL;
        break;
    }
}
void reg_read_vet(void *vet, unsigned int nRegs, unsigned int addr)
{
    switch (addr)
    {
    case Sys_RegMap_Offset_Bool ... (Sys_RegMap_Offset_Bool+Sys_RegMap_Nreg_Bool_tot):
        reg_read_bool_vet((bool*)vet, nRegs, addr);
        break;
    case Sys_RegMap_Offset_Short ... (Sys_RegMap_Offset_Short+Sys_RegMap_Nreg_Short_tot):
        reg_read_short_vet((short*)vet, nRegs, addr);
        break;
    case Sys_RegMap_Offset_Int ... (Sys_RegMap_Offset_Int+Sys_RegMap_Nreg_Int_tot):
        reg_read_int_vet((int*)vet, nRegs, addr);
        break;
#ifndef __NO_FLOAT__
    case Sys_RegMap_Offset_Float... (Sys_RegMap_Offset_Float+Sys_RegMap_Nreg_Float_tot):
        reg_read_float((float*)vet, nRegs, addr);
        break;
#endif
    default:
        break;
    }
}

/***********************************
 * Funções auxiliares
 ***********************************/
unsigned char *reg_ptr()
{
    return (char*)&reg_vet;
}
unsigned int reg_mem_size()
{
    return sizeof(reg_vet);
}

char reg_return_change_flag()
{
    return change_flag;
}
void reg_clear_change_flag(char flag)
{
    if (flag == NULL)
    {
        change_flag = 0;
        return;
    }
    change_flag = change_flag & ~flag;
}
