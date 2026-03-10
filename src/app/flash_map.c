#include "flash_map.h"
#include "board_defs.h"
#include <string.h>

/* ====== Checagens de tamanho (importante para layout estável) ====== */
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 201112L)
#  define STATIC_ASSERT(cond, msg) typedef char static_assert_##msg[(cond)?1:-1]
#else
#  define STATIC_ASSERT(cond, msg) _Static_assert((cond), #msg)
#endif

STATIC_ASSERT(sizeof(float)   == 4, float_must_be_4_bytes);
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
            Sys_equip_code, //Codigo do equipamento 
            Sys_firmware_version_major, Sys_firmware_version_minor, Sys_firmware_version_patch  //Versão do firmware    
        },
        /* ints[]: int32_t */
        .ints = {
            Sys_timestamp_min,
            Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, 
            Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, Sys_schedulers_default, 
            0,// Sys_RegMap_Flux_Counter,
            0,// Sys_RegMap_Flux_Liters,
            0,// Sys_RegMap_Total_Liters,
            0,// Sys_RegMap_Temp_S1,
            0,// Sys_RegMap_Adc_Measure_S1,
            0,// Sys_RegMap_mV_Measure_S1,
            0,// Sys_RegMap_Temp_S2,
            0,// Sys_RegMap_Adc_Measure_S2,
            0,// Sys_RegMap_mV_Measure_S2,
            3500,// Sys_RegMap_Temp_Ref_1,
            4500,// Sys_RegMap_Temp_Ref_1_Max,
            3500,// Sys_RegMap_Temp_Ref_2,
            4500,// Sys_RegMap_Temp_Ref_2_Max,
            100,// Sys_RegMap_Flux_Calib,
            1,// Sys_RegMap_T1_Calib_1,
            1,// Sys_RegMap_T1_Calib_2,
            1,// Sys_RegMap_T1_Calib_3,
            1,// Sys_RegMap_T2_Calib_1,
            1,// Sys_RegMap_T2_Calib_2,
            1,// Sys_RegMap_T2_Calib_3,
            6000,// Sys_RegMap_T1_Error_High,
            -2500,// Sys_RegMap_T1_Error_Low,
            100000,// Sys_RegMap_T1_Error_Desconnect,
            0,// Sys_RegMap_T1_Error_Short_Circuit,
            6000,// Sys_RegMap_T2_Error_High,
            -2500,// Sys_RegMap_T2_Error_Low,
            100000,// Sys_RegMap_T2_Error_Desconnect,
            0,// Sys_RegMap_T2_Error_Short_Circuit,
            0//Sys_RegMap_Erros
        },
        /* floats[]: float (32 bits) */
        .floats = {
        }
    }
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
uintptr_t nv_flash_addr_floats(void)
{
    return (uintptr_t)(&nv_defaults.data.floats[0]);
}

/* Tamanhos úteis (bytes) */
size_t nv_size_bools(void)  { return (sizeof(nv.bools)/8);  }
size_t nv_size_shorts(void) { return sizeof(nv.shorts); }
size_t nv_size_ints(void)   { return sizeof(nv.ints);   }
size_t nv_size_floats(void) { return sizeof(nv.floats); }