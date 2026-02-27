#include "flash_memory.h"
#include "fsl_iap.h"
#include <board.h>
#include "board_defs.h"

#ifdef SysEepromSize
    #define IAP_FLASH_SIZE SysEepromSize
#else
    #define IAP_FLASH_SIZE 128
#endif
#define iap_flash_end_sector    (FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES/FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES)
#define iap_flash_end_page      (FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES/FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)
#define iap_flash_sector        ((FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES-IAP_FLASH_SIZE)/FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES)
#define iap_flash_page_num      (IAP_FLASH_SIZE/FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)
#define iap_flash_page          ((FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES-IAP_FLASH_SIZE)/FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)
#define iap_flash_first_addr    (iap_flash_page*FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)

//vetor temporario para alocação dos valores que devem ser salvos na eeprom
static unsigned char iap_flash_Vet[IAP_FLASH_SIZE];
/**
 * @brief faz a inicialização do vetor temporário
 * 
 */
void iap_Begin()
{
    for (int i = 0; i < IAP_FLASH_SIZE; i++)
        iap_flash_Vet[i] = iap_ReadChar(i);
}
/**
 * @brief lê o valor presente no endereço de memória 
 * 
 * @param addr endereço a ser lido
 * @return unsigned char valor lido
 */
unsigned char iap_ReadChar(unsigned int addr)
{
    return *((unsigned char*)(addr + iap_flash_first_addr));
}
/**
 * @brief Lê um vetor na eeprom
 * 
 * @param vet       vetor que receberá o valor
 * @param size      tamanho do vetor que vai ser lido
 * @param addrInit  endereço inicial
 * @return unsigned char FlashOK
 */
unsigned char iap_ReadVet(unsigned char *vet, unsigned short size, unsigned int addrInit)
{
    memcpy(vet, (unsigned char*)(addrInit + iap_flash_first_addr),size);
    return FlashOK;
}
/**
 * @brief Lê um valor do tipo uInt16 no endereço enviado
 * 
 * @param addr Endereço
 * @return unsigned short valor lido
 */
unsigned short iap_ReadShort(unsigned int addr)
{
    return *((unsigned short*)(addr + iap_flash_first_addr));
}
/**
 * @brief Lê um valor do tipo uInt32 no endereço indicado
 * 
 * @param addr Endereço
 * @return unsigned int valor lido
 */
unsigned int iap_ReadInt(unsigned int addr)
{
    return *((unsigned int*)(addr + iap_flash_first_addr));
}
/**
 * @brief Escreve um vetor na memoria temporária
 * 
 * @param vet vetor de valores a serem escritos
 * @param size tamanho do vetor
 * @param addrInit endereço inicial a ser escrito
 * @return unsigned char 
 */
unsigned char iap_WriteVet(unsigned char *vet, unsigned short size, unsigned int addrInit)
{
    if (IAP_FLASH_SIZE < (addrInit+size))
        return FlashFail;
    for (int i = 0; i < size; i++)
        iap_flash_Vet[i+addrInit] = vet[i];
    return FlashOK;
}
/**
 * @brief Escreve unsigned char no vetor temporário
 * 
 * @param value valor a ser escrito
 * @param addr endereço
 * @return unsigned char 'FlashOK' ou 'FlashFail'
 */
unsigned char iap_WriteChar(unsigned char value, unsigned int addr)
{
    if (addr < IAP_FLASH_SIZE)
    {
        iap_flash_Vet[addr] = value;
        return FlashOK;
    }
    return FlashFail;
}
/**
 * @brief Escreve valor uInt16 no vetor temporário
 * 
 * @param value valor a ser escrito
 * @param addr endereço que será escrito
 * @return unsigned char 'FlashOK' ou 'FlashFail'
 */
unsigned char iap_WriteShort(unsigned short value, unsigned int addr)
{
    if((addr+1) < IAP_FLASH_SIZE)
    {
        for (unsigned char i = 0; i < sizeof(unsigned short); i++)
            iap_WriteChar((unsigned char) (value>>(8*i)), (addr+i));
        return FlashOK;
    }
    return FlashFail;
}
/**
 * @brief Escreve valor uInt32 no vetor temporário
 * 
 * @param value valor a ser escrito
 * @param addr endereço onde será escrito o valor
 * @return unsigned char  'FlashOK' ou 'FlashFail'
 */
unsigned char iap_RriteInt(unsigned int value, unsigned int addr)
{
    if((addr+3) < IAP_FLASH_SIZE)
    {
        for (unsigned char i = 0; i < sizeof(unsigned int); i++)
            iap_WriteChar((unsigned char) (value>>(8*i)), (addr+i));
        return FlashOK;
    }
    return FlashFail;
}
/**
 * @brief Faz o commit, escreve na memoria flash os valores presentes no vetor temporário
 * 
 * @return unsigned char 
 */
unsigned char iap_Commit()
{
    unsigned char test = 0;
    for (unsigned short i = 0; i < IAP_FLASH_SIZE; i++) //verifica se tem algum valor diferente para não desperdiçar
        if (iap_flash_Vet[i] != iap_ReadChar(i))        //o numero limitado de escritas que são permitidos na memória
        {
            test = 1;
            break;
        }
    if (test == 0)
        return FlashOK;
    //Prepara o setor para escrita (1024 bytes)
    IAP_PrepareSectorForWrite(iap_flash_sector, iap_flash_end_sector);
    //Limpa as paginas preparadas para escrita (64 bytes * n)
    //IAP_EraseSector(iap_flash_sector, FLASH_SIZE_sectors, SystemCoreClock);
    IAP_ErasePage(iap_flash_page, iap_flash_end_page, SystemCoreClock);
    for (int i = 0; i < iap_flash_page_num; i++)
    {
        IAP_PrepareSectorForWrite(iap_flash_sector, iap_flash_end_sector);
        IAP_CopyRamToFlash(iap_flash_first_addr + (FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES*i), 
                            (long unsigned int *)(&iap_flash_Vet[0] + (FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES*i)),
                            FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
    }
    return FlashOK;
}
/**
 * @brief reseta os valores presentesna flash 0xFF
 * 
 * @return unsigned char 
 */
unsigned char iap_Reset()
{
    for (size_t i = 0; i < IAP_FLASH_SIZE; i++)
        iap_flash_Vet[i] = 0xFF;
    iap_Commit();
    return FlashOK;
}