#ifndef FLASH_MAP_H
#define FLASH_MAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "board_defs.h"


#ifdef __cplusplus
extern "C" {
#endif

/* ========= TAMANHOS DOS VETORES =========
   Ajuste conforme seu projeto.
*/
#ifndef Sys_RegMap_Nreg_Bool_tot
#define Sys_RegMap_Nreg_Bool_tot   240
#endif

#ifndef Sys_RegMap_Nreg_Short_tot
#define Sys_RegMap_Nreg_Short_tot  120
#endif

#ifndef Sys_RegMap_Nreg_Int_tot
#define Sys_RegMap_Nreg_Int_tot    60
#endif

#ifndef __NO_FLOAT__
    #ifndef Sys_RegMap_Nreg_Float_tot
    #define Sys_RegMap_Nreg_Float_tot  60
    #endif
#endif

/* ========= TIPOS ========= */

typedef struct __attribute__((packed, aligned(4))) {
    uint8_t  bools[Sys_RegMap_Nreg_Bool_tot/8];     /* bool salvo como 1 byte */
    int16_t  shorts[Sys_RegMap_Nreg_Short_tot];
    int32_t  ints[Sys_RegMap_Nreg_Int_tot];
#ifndef __NO_FLOAT__
    float    floats[Sys_RegMap_Nreg_Float_tot];   /* 32 bits IEEE-754 */
#endif
} nv_payload_t;

typedef struct __attribute__((packed, aligned(4))) {
    uint32_t    magic;     /* p.ex.: 'NV00' = 0x4E563030 */
    uint16_t    version;   /* sua versão de layout */
    uint16_t    length;    /* sizeof(nv_payload_t) */
    uint32_t    crc32;     /* opcional (0 se não usar) */
    nv_payload_t data;     /* <<< BLOCO QUE VAMOS COPIAR PARA RAM */
} nv_image_t;

/* ========= OBJETOS GLOBAIS ========= */

/* Instância em RAM (alvo do memcpy) */
extern nv_payload_t nv;

/* Defaults em Flash (compilado como const) */
extern const nv_image_t nv_defaults;

/* ========= API ========= */

/* Copia os defaults da Flash para RAM (um memcpy).
 * Retorna 0 se OK, negativo se falhar em checagens básicas.
 */
int nv_load_defaults(void);

/* Endereços dos vetores na FLASH (defaults) — úteis se quiser IAP depois. */
uintptr_t nv_flash_addr_bools(void);
uintptr_t nv_flash_addr_shorts(void);
uintptr_t nv_flash_addr_ints(void);
uintptr_t nv_flash_addr_floats(void);

/* Tamanho em bytes de cada vetor (para operações de cópia/gravação) */
size_t nv_size_bools(void);
size_t nv_size_shorts(void);
size_t nv_size_ints(void);
size_t nv_size_floats(void);

#ifdef __cplusplus
}
#endif


#endif