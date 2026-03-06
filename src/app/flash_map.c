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
            /* Exemplo: 16 valores */
            1,0,1,0,  1,1,0,0,  1,0,0,1,  0,1,0,1
        },
        /* shorts[]: int16_t */
        .shorts = {
            100, 200, -50, 0,   10, 20, 30, 40,
            50,  60,  70, 80,   90, 100, -200, 300
        },
        /* ints[]: int32_t */
        .ints = {
            12345, -54321, 0, 111, 222, 333, 444, 555,
            666, 777, 888, 999, 1000, -1000, 42, -42
        },
        /* floats[]: float (32 bits) */
        .floats = {
            0.0f, 1.0f, -1.5f, 3.14f, 2.71f, 10.0f, -2.0f, 100.5f,
            0.25f, 0.5f, 0.75f, -0.25f, 5.5f, 6.5f, 7.5f, 8.5f
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
size_t nv_size_bools(void)  { return sizeof(nv.bools);  }
size_t nv_size_shorts(void) { return sizeof(nv.shorts); }
size_t nv_size_ints(void)   { return sizeof(nv.ints);   }
size_t nv_size_floats(void) { return sizeof(nv.floats); }