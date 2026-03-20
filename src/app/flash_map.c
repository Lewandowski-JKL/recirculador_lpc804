#include "flash_map.h"
#include "board_defs.h"
#include <string.h>

/* ====== Checagens de tamanho (importante para layout estável) ====== */
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 201112L)
#  define STATIC_ASSERT(cond, msg) typedef char static_assert_##msg[(cond)?1:-1]
#else
#  define STATIC_ASSERT(cond, msg) _Static_assert((cond), #msg)
#endif
#ifndef __NO_FLOAT__
STATIC_ASSERT(sizeof(float)   == 4, float_must_be_4_bytes);
#endif
STATIC_ASSERT(sizeof(int32_t) == 4, int32_must_be_4_bytes);
STATIC_ASSERT(sizeof(int16_t) == 2, int16_must_be_2_bytes);
STATIC_ASSERT(sizeof(uint8_t) == 1, u8_must_be_1_byte);

/* ====== Instância em RAM ====== */
nv_payload_t nv;

/* ====== Defaults em FLASH ======
 * Edite os valores iniciais aqui conforme sua necessidade.
 * Você pode deixar alguns zeros se preferir.
 */
__attribute__((section(".nv_defaults"), used, aligned(4)))
const nv_image_t nv_defaults = {
    .magic   = 0x4E563030u,      /* 'NV00' em ASCII */
    .version = 1u,
    .length  = sizeof(nv_payload_t),
    .crc32   = 0u,               /* (opcional) calcule depois se quiser validar */
    .data = {
        /* bools[]: 1 byte cada (0/1). */
        .bools = {
            /* valores utilizados no registrador     restante dos valores de fábrica é zero*/
            0b00000000                             
        },
        /* shorts[]: int16_t */
        .shorts = {
            Sys_equip_code,             // Sys_RegMap_Model     
            Sys_firmware_version_major, // Sys_RegMap_FV_Major     
            Sys_firmware_version_minor, // Sys_RegMap_FV_Minor      
            Sys_firmware_version_patch, // Sys_RegMap_FV_Patch
            0XFF,                       // Sys_RegMap_Mac_Addr_0
            0xFF,                       // Sys_RegMap_Mac_Addr_1
            0xFF,                       // Sys_RegMap_Mac_Addr_2
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_ssid_0
            0x0, 0x0, 0x0, 0x0,         //Define 0 como nome de rede padrão
            0x0, 0x0, 0x0, 0x0, 
            0x0, 0x0, 0x0, 0x0,    
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_pass_0
            0x0, 0x0, 0x0, 0x0,         //Define 0 como senha padrão
            0x0, 0x0, 0x0, 0x0, 
            0x0, 0x0, 0x0, 0x0,           
            //Os proximos endereços definem se o registrador deve ser salvo na eeprom 
            //booleanos
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_eeprom_reg_bool_0
            0x0, 0x0, 0x0, 0x0,         //Não salva nenhum booleano  
            //short
            0b0000111111111111,         // Sys_RegMap_eeprom_reg_short_0 
            0xFFFF, 0xFFFF, 0xFFFF,       
            0xFFFF, 0xFFFF, 
            0b1111111000000000, 0x0,         
            //int
            0b0111111111101111,         // Sys_RegMap_eeprom_reg_int_0
            0b1000111111111000, 
            0b1111111110001111,
            0b1110000000000000,        
            0x0, 0x0, 0x0, 0x0,            
            //float
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_eeprom_reg_float_0
            0x0, 0x0, 0x0, 0x0,         //Não salva nenhum booleano             
            //Os proximos endereços definem se o registrador tem prioridade de envio
            //isso quer dizer que qualquer alteração ele envia para os outros equipamentos
            //booleanos
            //Toda vez que a bomba ou o botão troca de valor ele enviar esse registrador
            0b1100000000000000,         // Sys_RegMap_priority_reg_bool_0
            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          
            //short
            //Os registradores short são definidos no inicio do codigo, não sofrem alteração
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_priority_reg_short_0 
            0x0, 0x0, 0x0, 0x0,         //Não salva nenhum booleano  
            //int
            //Alguns registradores int são medidas e devem ser enviados logo quando identifica uma mudança
            0b1111111111111110,         // Sys_RegMap_priority_reg_int_0
            0b0111110000000111, 
            0b0000111000000000, 
            0x0, 0x0, 0x0, 0x0, 0x0,   
            //float
            //Não utiliza registradores float
            0x0, 0x0, 0x0, 0x0,         // Sys_RegMap_priority_reg_float_0
            0x0, 0x0, 0x0, 0x0         //Não salva nenhum booleano    
        },
        /* ints[]: int32_t */
        //0b1111 1111 1111 1110
        //0b0111 1100 0000 0111
        //0b0000 1110 0000 0000
        .ints = {
            Sys_timestamp_min,
            Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, 
            Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, 
            //Registradores de leitura de fluxo
            0,// Sys_RegMap_Flux_Counter,
            0,// Sys_RegMap_Flux_Liters,
            0,// Sys_RegMap_Flux_Total_Liters,
            133,// Sys_RegMap_Flux_Calib,
            100000,// Sys_RegMap_Flux_Error_Max,
            0,// Sys_RegMap_Flux_Error_Min,
            //Registradores de leitura de temperatura s1
            0,// Sys_RegMap_S1_Adc,
            0,// Sys_RegMap_S1_mV,
            0,// Sys_RegMap_S1_Temp,
            3500,// Sys_RegMap_S1_Temp_Ref,
            200,//Sys_RegMap_S1_Temp_Hysteresis,
            1,// Sys_RegMap_S1_Calib_1,
            1,// Sys_RegMap_S1_Calib_2,
            1,// Sys_RegMap_S1_Calib_3,
            4500,// Sys_RegMap_S1_Error_High,
            0,// Sys_RegMap_S1_Error_Low,
            20000,// Sys_RegMap_S1_Error_Desconnect,
            0,// Sys_RegMap_S1_Error_Short_Circuit,
            //Registradores de leitura de temperatura s2
            0,// Sys_RegMap_S2_Adc,
            0,// Sys_RegMap_S2_mV,
            0,// Sys_RegMap_S2_Temp,
            3500,// Sys_RegMap_S2_Temp_Ref,
            200,//Sys_RegMap_S2_Temp_Hysteresis,
            1,// Sys_RegMap_S2_Calib_1,
            1,// Sys_RegMap_S2_Calib_2,
            1,// Sys_RegMap_S2_Calib_3,
            4500,// Sys_RegMap_S2_Error_High,
            0,// Sys_RegMap_S2_Error_Low,
            20000,// Sys_RegMap_S2_Error_Desconnect,
            0,// Sys_RegMap_S2_Error_Short_Circuit,
            //Registradores de leitura de corrente
            0,// Sys_RegMap_Current_Adc,
            0,// Sys_RegMap_Current_mV,
            0,// Sys_RegMap_Current,
            34,// Sys_RegMap_Current_Resistor,
            10000,// Sys_RegMap_Current_Calib,
            0,// Sys_RegMap_Current_Error_Desconnect,
            4000,// Sys_RegMap_Current_Error_Short_Circuit,
            //Configurações
            30000,// Sys_RegMap_Time_Recirculation,
            0,// Sys_RegMap_Temp_Ref_Recirculation,
            //Alarmes e erros
            0u// Sys_RegMap_Errors
#ifndef __NO_FLOAT__
        },
        /* floats[]: float (32 bits) */
        .floats = {
#endif
        }}
};


/* ====== API ====== */

int nv_load_defaults(void)
{
    /* Checagens simples para evitar copiar lixo por engano */
    if (nv_defaults.magic != 0x4E563030u) { /* 'NV00' */
        return -1;
    }
    if (nv_defaults.length != sizeof(nv_payload_t)) {
        return -2;
    }

    /* Se quiser, aqui você poderia validar o CRC32 de nv_defaults.data. */

    /* Cópia única do payload (Flash -> RAM) */
    memcpy(&nv, &nv_defaults.data, sizeof(nv));
    return 0;
}

/* Pointers/endereços dos arrays na FLASH (defaults) */
uintptr_t nv_flash_addr_bools(void)
{
    return (uintptr_t)(&nv_defaults.data.bools[0]);
}
uintptr_t nv_flash_addr_shorts(void)
{
    return (uintptr_t)(&nv_defaults.data.shorts[0]);
}
uintptr_t nv_flash_addr_ints(void)
{
    return (uintptr_t)(&nv_defaults.data.ints[0]);
}
#ifndef __NO_FLOAT__
    uintptr_t nv_flash_addr_floats(void)
    {
        return (uintptr_t)(&nv_defaults.data.floats[0]);
    }
#endif
/* Tamanhos úteis (bytes) */
size_t nv_size_bools(void)  { return (sizeof(nv.bools)/8);  }
size_t nv_size_shorts(void) { return sizeof(nv.shorts); }
size_t nv_size_ints(void)   { return sizeof(nv.ints);   }
#ifndef __NO_FLOAT__
    size_t nv_size_floats(void) { return sizeof(nv.floats); }
#endif