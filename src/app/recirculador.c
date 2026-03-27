#include <stdio.h>
#include "board_defs.h"
#include "recirculador.h"
#include "system.h"
#include "app_config.h"
#include "message_manager.h"
#include "registers_manager.h"
#include "calendar.h"

/*************************** */
__attribute__((weak)) bool reg_eeprom_test(unsigned int addr)
{
    /*switch (addr)
    {
    case Sys_RegMap_Mac_Addr_0 ... (Sys_RegMap_pass_0+16):
        return true;
    case Sys_RegMap_Schedules_0 ... Sys_RegMap_Schedules_9:
        return true;
    case Sys_RegMap_Total_Liters ... Sys_RegMap_T2_Calib_3:
        return true;
    default:
        return false;
    }*/
    return false;
}
/************************** */

/************************
 * Definições uteis
 *************************/
#define PUMP_ON     {   reg_write_bool(true, Sys_RegMap_Pump);\
                        gpio_ClearPin(Syspin_Pump);}
#define PUMP_OFF    {   reg_write_bool(false, Sys_RegMap_Pump);\
                        gpio_SetPin(Syspin_Pump);}

/************************
 * Funções uteis
 *************************/
bool rec_variation_test(int ref1, int ref2, int variation)
{
    int value = ref1-ref2;
    if ((value > (-variation)) && (value < variation))
        return false;
    return true;
}
int rec_average(int *vet, int size)
{
    int aux = 0;
    for (int i = 0; i < size; i++)
        aux+=vet[i];
    return (aux/size);
}


/*************************** */
typedef struct reg_eeprom_test_s
{
    int counter;
    int flag;
}reg_eeprom_test_s;
reg_eeprom_test_s reg_eeprom_test_var = 
{
    .counter = 0,
    .flag = 0
};
void rec_eeprom_test(void *arg)
{
    reg_eeprom_test_var.counter++;
    reg_eeprom_test_var.counter%=10;
    if ((!reg_eeprom_test_var.counter) || (!reg_eeprom_test_var.flag) )
        return;
    //salva na eeprom
    eeprom_WriteVet(reg_ptr(), reg_mem_size(), 0);
    //limpa os valores que devem ser limpos
}
/************************** */


typedef struct rec_get_temp_s
{
    int measure_adc_vet[3];
    int measure_mv_vet[3];
    int measure_temp_vet[3];
    int counter;
}rec_get_temp_s;
rec_get_temp_s rec_get_temp1_var = {
    .counter = 0
};
rec_get_temp_s rec_get_temp2_var =  {
    .counter = 0
};
void rec_get_Temp(void *arg)
{
    rec_get_temp1_var.counter++;
    rec_get_temp1_var.counter%=(sizeof(rec_get_temp1_var.measure_adc_vet)/sizeof(int));
    //Faz a leitura do adc, converte em mV apos isso converte em °C
    int measure_aux = adc_Read(&adc_S1);
    rec_get_temp1_var.measure_adc_vet[rec_get_temp1_var.counter] = measure_aux;
    measure_aux = adc_ConvertToMiliVolts(&adc_S1, measure_aux);
    rec_get_temp1_var.measure_mv_vet[rec_get_temp1_var.counter] = measure_aux;
    rec_get_temp1_var.measure_temp_vet[rec_get_temp1_var.counter] = convert_mV_to_celcius(&temp_S1,measure_aux);

    rec_get_temp2_var.counter++;
    rec_get_temp2_var.counter%=(sizeof(rec_get_temp2_var.measure_adc_vet)/sizeof(int));
    //Faz a leitura do adc, converte em mV apos isso converte em °C
    measure_aux = adc_Read(&adc_S2);
    rec_get_temp2_var.measure_adc_vet[rec_get_temp2_var.counter] = measure_aux;
    measure_aux = adc_ConvertToMiliVolts(&adc_S2, measure_aux);
    rec_get_temp2_var.measure_mv_vet[rec_get_temp2_var.counter] = measure_aux;
    rec_get_temp2_var.measure_temp_vet[rec_get_temp2_var.counter] = convert_mV_to_celcius(&temp_S2,measure_aux);
}
void rec_ref_measure(void* arg)
{
    int measure_aux = adc_Read(&adc_Vref);
    measure_aux = 2500000/measure_aux;//novo valor de conversão
    adc_S1._convertTo_mv = measure_aux;
    adc_S2._convertTo_mv = measure_aux;
    adc_Current._convertTo_mv = measure_aux;
}

typedef struct isr_Botoeira_s
{
    unsigned long long last_time_click;
    bool pump_on;
    unsigned int debouce_time;
}isr_Botoeira_s;
isr_Botoeira_s isr_botoeira_var = 
{
    .last_time_click = 0,
    .pump_on = false,
    .debouce_time = 20*SysTicksButtonDebounce //100ms
};
void rec_isr_Botoeira(volatile void *arg)
{
    //faz o teste de debounce para evitar ruido 100ms
    if ((isr_botoeira_var.last_time_click+isr_botoeira_var.debouce_time) > SysTickGetTime_ms())
        return;
    if(gpio_ReadPin(Syspin_Botoeira))//verifica se o botão continua pressionado 
        return;
    //Salva tempo atual para uso no proximo teste de debounce
    isr_botoeira_var.last_time_click = SysTickGetTime_ms();
    isr_botoeira_var.pump_on = !isr_botoeira_var.pump_on;
    //Apenas um controle para identificar se a bomba foi ligada através do botão
    if (isr_botoeira_var.pump_on)
    {
        PUMP_ON;
    }else
    {
        PUMP_OFF;
    }
}

/**
 * @brief Tratamento da interrupção da entrada de sensor de fluxo
 * 
 * @param arg 
 */
typedef struct isr_flow_s
{
    int counter;
}isr_flow_s;
isr_flow_s isr_flow_var = 
{
    .counter = 0
};
void rec_isr_Flow(volatile void *arg)
{
    // //Essa leitura Auxilia como um filtro de ruido
    // if(gpio_ReadPin(Syspin_SensorDeFluxo))
    //     return;
    //Incrementa contador de fluxo
    isr_flow_var.counter++;
}

typedef struct rec_current_s
{
    int offset;
}rec_current_s;
rec_current_s rec_current_var = 
{
    .offset = 0,
};
void rec_current_measure(void* arg)
{
    int *ptrAux = (int*)reg_ptr_int(0);
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current_Adc)] = adc_Read(&adc_Current);
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current_mV)] = adc_ConvertToMiliVolts(&adc_Current, 
                                                        ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current_Adc)]);
    //se a bomba estiver desligada isso é o offset
    int vAux = (1000*ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current_mV)]) /
                ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current_Resistor)];
    if (reg_read_bool(Sys_RegMap_Pump))
    {
        ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current)] = vAux - rec_current_var.offset;
    }else
        {
            rec_current_var.offset = vAux;
            ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current)] = 0;
        }
}

void rec_measure(void *arg)
{
    int *ptrAux = (int*)reg_ptr_int(0);
    //Faz a média das medidas e carrega nos registradores do modbus
    int size_vet = (sizeof(rec_get_temp1_var.measure_adc_vet)/sizeof(int));
    int temp_aux = rec_average(rec_get_temp1_var.measure_temp_vet, size_vet);
    // //Medidas de temperatura do sensor 1
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S1_Adc)] = rec_average(rec_get_temp1_var.measure_adc_vet, size_vet); 
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S1_mV)]= rec_average(rec_get_temp1_var.measure_mv_vet, size_vet); 
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S1_Temp)] = temp_aux;

    //Medidas de temperatura do sensor 2
    temp_aux = rec_average(rec_get_temp2_var.measure_temp_vet, size_vet);
    // //Medidas de temperatura do sensor 2
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S2_Adc)] = rec_average(rec_get_temp2_var.measure_adc_vet, size_vet); 
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S2_mV)]= rec_average(rec_get_temp2_var.measure_mv_vet, size_vet); 
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S2_Temp)] = temp_aux;

    //Medidas de temperatura do sensor de Fluxo
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Counter)] = isr_flow_var.counter;
    isr_flow_var.counter = 0;
    int value_aux = (ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Calib)] * ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Counter)]);
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Total_Liters)] += value_aux;
    ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Liters)] = 60*value_aux;
}

typedef struct rec_create_messages_s
{
    int S1;
    int S2;
    int Flux;
    int Current;
    int Timestamp;
    unsigned char Message_counter;
}rec_create_messages_s;
rec_create_messages_s rec_create_messages_var = 
{
    .S1 = 0,
    .S2 = 0,
    .Flux = 0,
    .Current = 0,
    .Timestamp = 0,
    .Message_counter = 0
};
void rec_create_messages(void *arg)
{
    /*rec_create_messages_var.Message_counter++;
    rec_create_messages_var.Message_counter%=5;
    if (!rec_create_messages_var.Message_counter)
    {
        rec_get_status(NULL);
    }*/
    rec_get_status(NULL);

    int *ptrAux = (int*)reg_ptr_int(0);
    char message_flag = 0b0;
    message_flag |= rec_variation_test(ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S1_Temp)], rec_create_messages_var.S1, 20)         ? 0b0001 : 0b0;
    message_flag |= rec_variation_test(ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S2_Temp)], rec_create_messages_var.S2, 20)         ? 0b0010 : 0b0;
    message_flag |= rec_variation_test(ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Liters)], rec_create_messages_var.Flux, 50)   ? 0b0100 : 0b0;
    message_flag |= rec_variation_test(ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current)], rec_create_messages_var.Current, 100)   ? 0b1000 : 0b0;
    message_flag |= (ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Timestamp)] != rec_create_messages_var.Timestamp)  ? 0b1000 : 0b0;
    if (!message_flag)
        return;
    rec_create_messages_var.S1 = ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S1_Temp)];
    rec_create_messages_var.S2 = ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_S2_Temp)];
    rec_create_messages_var.Flux = ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Flux_Liters)];
    rec_create_messages_var.Current = ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Current)];
    rec_create_messages_var.Timestamp = ptrAux[Sys_RegMap_GetIndex(Sys_RegMap_Timestamp)];
    i2c_modbus_s message;
    message.code = Sys_Modbus_Set_Mult_Num_Reg;
    message.RegAddr = Sys_RegMap_Offset_Int;
    message.nReg = (Sys_RegMap_Errors - Sys_RegMap_Timestamp);
    message.bytes = (message.nReg * sizeof(int));
    message.ptrMessage = reg_ptr_int(0);
    message.addr = SysI2C_ADDR_ALL;
    message.size_head = 6;
    message.size_message = message.bytes;
    message_New(message);
}
/***********************
 * Variaveis Sys
***********************/
/**
 * Faz a gestão e controle da bomba dependendo do que a acionar
 */
#ifndef __LOW_MEMORY__
typedef struct req_verify_send
{
    char var_on;
    bool *bool_var;
    short *short_var;
    int *int_var;
}req_verify_send;
req_verify_send req_verify_send_var =
{
    .var_on = 0,
    .bool_var = NULL,
    .int_var = NULL
};
void _rec_chage_begin_var()
{
    req_verify_send_var.var_on = 1;
    short regBool = 0;
    short regShort = 0;
    short regInt = 0;
    int nRegBool = 0;
    int nRegShort = 0;
    int nRegInt = 0;
    short bit = 0b1000000000000000;
    for (int i = 0; i < 8; i++)
    {
        regBool = 0;
        regShort = 0;
        regInt = 0;
    }
    bool *ptrBool = (bool*)malloc(regBool);
    short *ptrShort = (short*)malloc(regShort);
    int *ptrInt = (int*)malloc(regInt);
    req_verify_send_var.bool_var = ptrBool;
    req_verify_send_var.short_var = ptrShort;
    req_verify_send_var.int_var = ptrInt;
}
void rec_change_verify(void *arg)
{
    if (!req_verify_send_var.var_on)
        _rec_chage_begin_var();
    
    // char *ptr = reg_ptr();
    short bit = 0b1000000000000000;
    int addr_aux = Sys_RegMap_priority_reg_bool_0;
    short bit_ref;
    //Primeiro verifica os bool 
    for (int i = 0; i < 8; i++)
    {
        bit_ref = reg_read_short(Sys_RegMap_priority_reg_bool_0+i);
        bit = 0b1000000000000000;
        for (int j = 0; i < 16; i++)
        {
            bit_ref = reg_read_short(Sys_RegMap_priority_reg_bool_0+i);
            //deve ser comparado
            if (bit & bit_ref)
            {
                /* code */
            }
            bit = bit >> 1;
        }
    }
}

typedef struct req_verify_send
{
    char bool_var;
    short short_var[Sys_RegMap_Nreg_Short];
    int int_var[Sys_RegMap_Nreg_Int];
}req_verify_send;
req_verify_send req_verify_send_var;
bool _is_different(void *ptrRef, void *ptrCompare, int size)
{
    for (int i = 0; i < size; i++)
        if((*(char*)(ptrRef+i)) != (*(char*)(ptrCompare+i)))
            return true;
    return false;
}

void rec_change_verify(void *arg)
{
    //Compara os booleanos
    char *ptr = reg_ptr();
    if (_is_different((void*)(&req_verify_send_var.bool_var), (void*)ptr, 1))
    {
        //Cria mensagem e copia
        memcpy((void*)(&req_verify_send_var.bool_var), (void*)ptr, 1);
    }
    //Compara os Shorts
    ptr = (reg_ptr()+(Sys_RegMap_Nreg_Bool_tot/8));
    if (_is_different((void*)(&req_verify_send_var.short_var), (void*)ptr, (Sys_RegMap_Nreg_Short*sizeof(short))))
    {
        //Cria mensagem e copia
        memcpy((void*)(&req_verify_send_var.short_var), (void*)ptr, (Sys_RegMap_Nreg_Short*sizeof(short)));
    }
    //Compara os ints
    ptr = (reg_ptr()+(Sys_RegMap_Nreg_Bool_tot/8)+Sys_RegMap_Nreg_Short_tot);
    if (_is_different((void*)(&req_verify_send_var.int_var), (void*)ptr, (Sys_RegMap_Nreg_Int*sizeof(int))))
    {
        //Cria mensagem e copia
        memcpy((void*)(&req_verify_send_var.int_var), (void*)ptr, (Sys_RegMap_Nreg_Int*sizeof(int)));
    }
}


#else
typedef struct _rec_change_s
{
    unsigned long long last_update;
    unsigned long long variation;
    char flag;
}_rec_change_s;
_rec_change_s _rec_change_var =
{
    .last_update = 0,
    .variation = 0,
    .flag = 0b1
};
void _rec_change()
{
    _rec_change_var.variation = (SysTickGetTime_ms() - _rec_change_var.last_update);
    if (_rec_change_var.variation > 10000)
    {
        _rec_change_var.last_update = SysTickGetTime_ms();
        _rec_change_var.variation = 0;
        _rec_change_var.flag = _rec_change_var.flag << 1;
    }
    if (_rec_change_var.flag & 0b10)
    {
        reg_set_change_flag(Reg_Change_Bool_Register);
        _rec_change_var.flag = _rec_change_var.flag << 1;
        return;
    }
    
    if (_rec_change_var.flag & 0b1000)
    {
        reg_set_change_flag(Reg_Change_Short_Register);
        _rec_change_var.flag = _rec_change_var.flag << 1;
        return;
    }
    
    if (_rec_change_var.flag & 0b100000)
    {
        reg_set_change_flag(Reg_Change_Int_Register);
        _rec_change_var.flag = 0b1;
        return;
    }

}
void rec_change_verify(void *arg)
{
    _rec_change();
    char flag_change = reg_return_change_flag();
    i2c_modbus_s message;
    if (flag_change & Reg_Change_Bool_Register)
    {
        /* Cria mensagem com os registradores booleanos */
        message.code = Sys_Modbus_Set_Mult_Num_Reg;
        message.RegAddr = Sys_RegMap_Offset_Bool;
        message.nReg = Sys_RegMap_Nreg_Bool;
        message.bytes = (Sys_RegMap_Nreg_Bool/8)+1;
        message.ptrMessage = reg_ptr_bool();
        message.addr = SysI2C_ADDR_ALL;
        message.size_head = 6;
        message.size_message = message.bytes;
        message_New(message);
        reg_clear_change_flag(Reg_Change_Bool_Register);
        return;
    }
    if (flag_change & Reg_Change_Short_Register)
    {
        /* Cria mensagem com os registradores Short */
        message.code = Sys_Modbus_Set_Mult_Num_Reg;
        message.RegAddr = Sys_RegMap_Offset_Short;
        message.nReg = Sys_RegMap_Nreg_Short;
        message.bytes = (Sys_RegMap_Nreg_Short*sizeof(short));
        message.ptrMessage = reg_ptr_short(0);
        message.addr = SysI2C_ADDR_ALL;
        message.size_head = 6;
        message.size_message = message.bytes;
        message_New(message);
        reg_clear_change_flag(Reg_Change_Short_Register);
        return;
    }
    if (flag_change & Reg_Change_Int_Register)
    {
        /* Cria mensagem com os registradores Int */
        message.code = Sys_Modbus_Set_Mult_Num_Reg;
        message.RegAddr = Sys_RegMap_Flux_Calib;
        message.nReg = (Sys_RegMap_Schedules_9 - Sys_RegMap_Flux_Calib)+1;
        message.bytes = (message.nReg*sizeof(int));
        message.ptrMessage = reg_ptr_int(Sys_RegMap_Flux_Calib);
        message.addr = SysI2C_ADDR_ALL;
        message.size_head = 6;
        message.size_message = message.bytes;
        message_New(message);
        reg_clear_change_flag(Reg_Change_Int_Register);
        return;
    }
}
#endif
void rec_pump_control()
{
    //Verifica se está em erro, caso estiver desliga a bomba e não executa as comparações
    if (reg_read_int(Sys_RegMap_Errors))
    {
        PUMP_OFF;
        return;
    }
    //Verifica se o Registrador está apontando para ligar a bomba e, se a bomba está ligada
    //Caso a bomba não esteja (nivél logico alto na saída) liga a bomba
    if (reg_read_bool(Sys_RegMap_Pump) && gpio_ReadPin(Syspin_Pump))
        PUMP_ON;
        
    int temp_measure = (reg_read_int(Sys_RegMap_Temp_Ref_Recirculation) == 0) ? 
                        reg_read_int(Sys_RegMap_S1_Temp) : reg_read_int(Sys_RegMap_S2_Temp);
    int temp_ref_sup = (reg_read_int(Sys_RegMap_Temp_Ref_Recirculation) == 0) ? 
                        reg_read_int(Sys_RegMap_S1_Temp_Ref) : reg_read_int(Sys_RegMap_S2_Temp_Ref);
    int temp_ref_inf = temp_ref_sup;
    temp_ref_sup += (reg_read_int(Sys_RegMap_Temp_Ref_Recirculation) == 0) ? 
                        reg_read_int(Sys_RegMap_S1_Temp_Hysteresis) : reg_read_int(Sys_RegMap_S2_Temp_Hysteresis);

    //Se atingiu a temperatura ele desliga a bomba
    if (temp_measure > temp_ref_sup)
    {
        PUMP_OFF;
        //Caso foi a partir da botoeira ele limpa a flag
        if (isr_botoeira_var.pump_on)
            isr_botoeira_var.pump_on = false;
        return;
    }

    //Verifica se a bomba está ligada por conta da botoeira
    if (isr_botoeira_var.pump_on )
    {
        //Verifica quanto tempo faz que foi pressionado, se estourou o tempo volta para a operação comum.
        unsigned long long dt_botoeira = SysTickGetTime_ms() - isr_botoeira_var.last_time_click;
        //Se dt maior que o tempo de recirculação ele desliga a bomba
        if (dt_botoeira > reg_read_int(Sys_RegMap_Time_Recirculation))
        {
            PUMP_OFF;
            isr_botoeira_var.pump_on = false;
        }
    }
    temp_ref_inf -= (reg_read_int(Sys_RegMap_Temp_Ref_Recirculation) == 0) ? 
                        reg_read_int(Sys_RegMap_S1_Temp_Hysteresis) : reg_read_int(Sys_RegMap_S2_Temp_Hysteresis);
    
    // //Verifica se está com um agendamento ativado
    if (schedulingTest())
    {
        if (temp_measure > temp_ref_sup)
        {
            PUMP_ON;
            return;
        }else if (temp_measure < temp_ref_inf)
        {
            PUMP_OFF;
            return;
        }   
    }
}
/*typedef struct _rec_ResePin_s
{
    char resetCounter;
}_rec_ResePin_s;
_rec_ResePin_s _rec_ResetPin_var =
{
    .resetCounter=0
};*/
void rec_system(void *arg)
{
    //faz a leitura para acompanhar se o botão ta pressionado
    reg_write_bool(!gpio_ReadPin(Syspin_Botoeira), Sys_RegMap_Button);
    /*if (gpio_ReadPin(Syspin_RESET))
    {
        reg_write_bool(false, Sys_RegMap_Reset_Button);
        _rec_ResetPin_var.resetCounter = 0;
    }else
        {
            reg_write_bool(true, Sys_RegMap_Reset_Button);
            _rec_ResetPin_var.resetCounter++;
            if (_rec_ResetPin_var.resetCounter > 150)
                gpio_resetEnable();
        }*/
    //Faz a gestão do controle da bomba
    rec_pump_control();   
}
int _rec_S1_error_test()
{
    int measure = reg_read_int(Sys_RegMap_S1_Temp);
    if (measure > reg_read_int(Sys_RegMap_S1_Error_Desconnect))
        return Sys_ERROR_S1_OPEN_CIRCUIT;
    if (measure < reg_read_int(Sys_RegMap_S1_Error_Short_Circuit))
        return Sys_ERROR_S1_SHORT_CIRCUIT;
    if (measure > reg_read_int(Sys_RegMap_S1_Error_High))
        return Sys_ERROR_S1_HIGH;
    if (measure < reg_read_int(Sys_RegMap_S1_Error_Low))
        return Sys_ERROR_S1_LOW;
    return Sys_ERROR_NO;
}
int _rec_S2_error_test()
{
    int measure = reg_read_int(Sys_RegMap_S2_Temp);
    if (measure > reg_read_int(Sys_RegMap_S2_Error_Desconnect))
        return Sys_ERROR_S1_OPEN_CIRCUIT;
    if (measure < reg_read_int(Sys_RegMap_S2_Error_Short_Circuit))
        return Sys_ERROR_S1_SHORT_CIRCUIT;
    if (measure > reg_read_int(Sys_RegMap_S2_Error_High))
        return Sys_ERROR_S1_HIGH;
    if (measure < reg_read_int(Sys_RegMap_S2_Error_Low))
        return Sys_ERROR_S1_LOW;
    return Sys_ERROR_NO;
}
int _rec_Current_error_test()
{
    /*
    
    int measure = reg_read_int(Sys_RegMap_S1_Temp);
    if (measure > reg_read_int(Sys_RegMap_S1_Error_Desconnect))
        return Sys_ERROR_S1_OPEN_CIRCUIT;
    if (measure < reg_read_int(Sys_RegMap_S1_Error_Short_Circuit))
        return Sys_ERROR_S1_SHORT_CIRCUIT;
    if (measure > reg_read_int(Sys_RegMap_S1_Error_High))
        return Sys_ERROR_S1_HIGH;
    if (measure < reg_read_int(Sys_RegMap_S1_Error_Low))
        return Sys_ERROR_S1_LOW;
    return Sys_ERROR_NO;
    */
    // ///////////////////////////////////////////////////
    // #define Sys_ERROR_CURRENT_HIGH           0b000000010000000000000000
    // #define Sys_ERROR_CURRENT_LOW            00000000100000000000000000
    // #define Sys_ERROR_CURRENT_SHORT_CIRCUIT  0b000010000000000000000000
    // #define Sys_ERROR_CURRENT_OPEN_CIRCUIT   0b000100000000000000000000
    return Sys_ERROR_NO;
}
void rec_error_process(void *arg)
{
    int value = Sys_ERROR_NO;
    value |= _rec_S1_error_test();
    value |= _rec_S2_error_test();
    value |= _rec_Current_error_test();
    reg_write_int(value, Sys_RegMap_Errors);
}
void rec_get_status(void *arg)
{
    /* Cria mensagem que atualiza o status dos outros dispositivos*/
    i2c_modbus_s message;
    message.code = Sys_Modbus_Report_Slave_ID;
    message.addr = SysI2C_ADDR_ALL;
    message.size_head = 1;
    message.size_message = 0;
    message_New(message);
    return;
}
