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

/*************************** */
bool _reg_eeprom_test(unsigned int addr)
{
    switch (addr)
    {
    case Sys_RegMap_Mac_Addr_0 ... (Sys_RegMap_pass_0+16):
        return true;
    case Sys_RegMap_Schedules_0 ... Sys_RegMap_Schedules_9:
        return true;
    case Sys_RegMap_Total_Liters ... Sys_RegMap_T2_Calib_3:
        return true;
    default:
        return false;
    }
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
    memcpy((unsigned char*)(reg_vet._bool_reg_vet), (unsigned char*)&__reg_factory , Sys_RegMap_Total_Bytes);
    //agora deve salvar toda a memória
    iap_WriteVet((unsigned char*)(reg_vet._bool_reg_vet), Sys_RegMap_Total_Bytes, 0);
    iap_Commit();
}

void reg_write_bool(bool value, unsigned int addr)
{
    
}
void reg_write_bool_vet(unsigned char *vet, unsigned int size, unsigned int addr)
{

}

void reg_write_short(short value, unsigned int addr)
{

}
void reg_write_short_vet(short *vet, unsigned int size, unsigned int addr)
{

}
void reg_write_int(int value, unsigned int addr)
{
    
}
void reg_write_int_vet(int *vet, unsigned int size, unsigned int addr)
{

}

#ifndef __NO_FLOAT__
void reg_write_float(float value, unsigned int addr)
{
    
}
void reg_write_float_vet(float *vet, unsigned int size, unsigned int addr)
{

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
        reg_write_bool_vet((*(bool*)vet), nRegs, addr);
        break;
    case Sys_RegMap_Offset_Short ... (Sys_RegMap_Offset_Short+Sys_RegMap_Nreg_Short_tot):
        reg_write_short_vet((*(short*)vet), nRegs, addr);
        break;
    case Sys_RegMap_Offset_Int ... (Sys_RegMap_Offset_Int+Sys_RegMap_Nreg_Int_tot):
        reg_write_int_vet((*(int*)vet), nRegs, addr);
        break;
#ifndef __NO_FLOAT__
    case Sys_RegMap_Offset_Float... (Sys_RegMap_Offset_Float+Sys_RegMap_Nreg_Float_tot):
        reg_write_float((*(float*)vet), nRegs, addr);
        break;
#endif
    default:
        break;
    }
}
