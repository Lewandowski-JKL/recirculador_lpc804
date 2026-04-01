/**
 * @file system.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-02-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "board.h"
#include "system.h"
#include "log_lpc804.h"
#define TAG "SYSTEM"

#define __SHRT_MIN__    (0xFFFF-__SHRT_MAX__)
#define __INT_MIN__     (0xFFFFFFFF-__INT_MAX__)

// a função void set_fro_frequency(uint32_t iFreq); não funcionou, aqui esta a nossa versão
/**
 * @brief Set the Clock30 M object
 * 
 */
void SysConfigClock30M()
{
    asm volatile(                      
    " ldr R1, ROM_Tbl \n"    // obtem o endereço do ponteiro da tabelea de drivers
    " ldr R1, [R1] \n"            //carrega o ponteiro
    " add R1, #0x0C \n"       // avança até a entrada da tabela FR0
    " ldr R1, [R1] \n"             // pega o endereço
    " add R1, #8 \n"             // avança até a terceira entrada na tabela
    " ldr R1, [R1] \n"             //obtem o endereço da rotina set_fro_frequency 
    " ldr R0, FRO_Freq30M \n"    // coloca a frequencia em R0
    " blx R1 \n"                      // Chama a api
    " b Exit \n"                      // Fim

    " .align 4 \n"                   // Alinha a memoria para garantir o alinhamento correto
    "FRO_Freq30M: .word 30000 \n" // frequencia em KHz (30MHz)
    "ROM_Tbl: .word 0x0F001FF8 \n" // Endereço da tabela de drivers da ROM

    "Exit: \n"                         // Fim do segmento do codigo
    );
    SystemCoreClock = 15000000;
}
/**
 * @brief Set the Clock24 M object
 * 
 */
void SysConfigClock24M()
{
    asm volatile(                      
    " ldr R1, ROM_Tbl24M \n"    // obtem o endereço do ponteiro da tabelea de drivers
    " ldr R1, [R1] \n"            //carrega o ponteiro
    " add R1, #0x0C \n"       // avança até a entrada da tabela FR0
    " ldr R1, [R1] \n"             // pega o endereço
    " add R1, #8 \n"             // avança até a terceira entrada na tabela
    " ldr R1, [R1] \n"             //obtem o endereço da rotina set_fro_frequency 
    " ldr R0, FRO_Freq24M \n"    // coloca a frequencia em R0
    " blx R1 \n"                      // Chama a api
    " b Exit \n"                      // Fim

    " .align 4 \n"                   // Alinha a memoria para garantir o alinhamento correto
    "FRO_Freq24M: .word 24000 \n" // frequencia em KHz (24MHz)
    "ROM_Tbl24M: .word 0x0F001FF8 \n" // Endereço da tabela de drivers da ROM

    "Exit24M: \n"                         // Fim do segmento do codigo
    );
    SystemCoreClock = 12000000;
}
/**
 * @brief Set the Clock18 M object
 * 
 */
void SysConfigClock18M()
{
    asm volatile(                      
    " ldr R1, ROM_Tbl18M \n"    // obtem o endereço do ponteiro da tabelea de drivers
    " ldr R1, [R1] \n"            //carrega o ponteiro
    " add R1, #0x0C \n"       // avança até a entrada da tabela FR0
    " ldr R1, [R1] \n"             // pega o endereço
    " add R1, #8 \n"             // avança até a terceira entrada na tabela
    " ldr R1, [R1] \n"             //obtem o endereço da rotina set_fro_frequency 
    " ldr R0, FRO_Freq18M \n"    // coloca a frequencia em R0
    " blx R1 \n"                      // Chama a api
    " b Exit \n"                      // Fim

    " .align 4 \n"                   // Alinha a memoria para garantir o alinhamento correto
    "FRO_Freq18M: .word 18000 \n" // frequencia em KHz (18MHz)
    "ROM_Tbl18M: .word 0x0F001FF8 \n" // Endereço da tabela de drivers da ROM

    "Exit18M: \n"                         // Fim do segmento do codigo
    );
    SystemCoreClock = 9000000;
}
/**
 * @brief 
 * 
 * @param time in ms
 */
void watchdog_init(int time)
{
    
    // Fonte do clock (LFRO recomendado)
    // Ativa o clock de do LFRO como 1Mhz como referencia
    // No watchdog tem um preescaler de 1/4 então o clock do contador é 250Khz
    SYSCON->PDRUNCFG &=	~(SYSCON_PDRUNCFG_LPOSC_PD_MASK);
    // Clock para o WDT
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_WWDT_MASK);
    // Timeout
    // Primeiro deve-se definir o valor de estouro
    // 1ms = 250
    WWDT->TC = 250*time;
    //WWDT->WINDOW = 0xFFFFFF;
    //WWDT->WARNINT = 0x00;
    // Enable + Reset mode
    WWDT->MOD = (1 << 0) | (1 << 1); // WDEN + WDRESET

    // Feed inicial
    WWDT->FEED = 0xAA;
    WWDT->FEED = 0x55;
}
void watchdog_feed(void)
{
    WWDT->FEED = 0xAA;
    WWDT->FEED = 0x55;
}
