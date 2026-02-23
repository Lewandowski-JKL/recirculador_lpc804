#include "myFlashMemory.h"
#include "${ProjDirPath}/../__repo__/core/drivers/iap/fsl_iap.h"
#include <board.h>


#define FLASH_SIZE          (0x4000)
#define FLASH_SIZE_sectors  (FLASH_SIZE/1024)
#define FLASH_SIZE_pages    (FLASH_SIZE/64)

#define iap_flash_sector        (15)
#define iap_flash_page_num      (IAP_FLASH_SIZE/FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)
#define iap_flash_page          ((FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES) * iap_flash_sector)
#define iap_flash_first_addr    (iap_flash_page*FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES)


static unsigned char iap_flash_Vet[IAP_FLASH_SIZE];

void iap_initFlash()
{
    for (int i = 0; i < IAP_FLASH_SIZE; i++)
        iap_flash_Vet[i] = iap_readChar(i);
}
unsigned char iap_readChar(unsigned int addr)
{
    return *((unsigned char*)(addr + iap_flash_first_addr));
}
unsigned short iap_readShort(unsigned int addr)
{
    unsigned short value = 0;
    for (unsigned char i = 0; i < sizeof(unsigned short); i++)
        value = (*((unsigned char*)(i+addr+iap_flash_first_addr))) << (i*8);
    return value;
}
unsigned int iap_readInt(unsigned int addr)
{
    unsigned int value = 0;
    for (unsigned char i = 0; i < sizeof(unsigned int); i++)
        value = (*((unsigned char*)(i+addr+iap_flash_first_addr))) << (i*8);
    return value;
}
unsigned char iap_writeChar(unsigned char value, unsigned int addr)
{
    if (addr < IAP_FLASH_SIZE)
    {
        iap_flash_Vet[addr] = value;
        return FlashOK;
    }
    return FlashFail;
}
unsigned char iap_writeShort(unsigned short value, unsigned int addr)
{
    if((addr+1) < IAP_FLASH_SIZE)
    {
        for (unsigned char i = 0; i < sizeof(unsigned short); i++)
            iap_writeChar((unsigned char) (value>>(8*i)), (addr+i));
        return FlashOK;
    }
    return FlashFail;
}
unsigned char iap_writeInt(unsigned int value, unsigned int addr)
{
    if((addr+3) < IAP_FLASH_SIZE)
    {
        for (unsigned char i = 0; i < sizeof(unsigned int); i++)
            iap_writeChar((unsigned char) (value>>(8*i)), (addr+i));
        return FlashOK;
    }
    return FlashFail;
}
unsigned char iap_commit()
{
    //Prepara o setor para escrite (1024 bytes)
    IAP_PrepareSectorForWrite(iap_flash_sector, FLASH_SIZE_sectors);
    //Limpa as paginas preparadas para escrita (64 bytes * n)
    IAP_EraseSector(iap_flash_sector, FLASH_SIZE_sectors, SystemCoreClock);
    //IAP_ErasePage(iap_flash_page, FLASH_SIZE_pages, SystemCoreClock);
    long unsigned int PageBufAux[IAP_FLASH_SIZE / sizeof(unsigned int)];
    for (int i = 0; i < IAP_FLASH_SIZE; i++)
        *(((unsigned char *)(&PageBufAux[0])) + i) = iap_flash_Vet[i];

    for (int i = 0; i < iap_flash_page_num; i++)
    {
        IAP_PrepareSectorForWrite(iap_flash_sector, iap_flash_sector);
        IAP_CopyRamToFlash(iap_flash_first_addr + (FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES*i), 
                                &PageBufAux[0], FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
    }
    
    return FlashOK;
}

