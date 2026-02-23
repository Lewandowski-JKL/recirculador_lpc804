#include "myFlashMemory.h"
#include "${ProjDirPath}/../__repo__/core/drivers/iap/fsl_iap.h"
#include <board.h>


#define FLASH_SIZE          (0x4000)
#define FLASH_SIZE_sectors  (FLASH_SIZE/1024)
#define FLASH_SIZE_pages    (FLASH_SIZE/64)

/*
#define IAP_FLASH_SECTOR            (15)
#define INVERSED_FLASH_ERASED_VALUE (1)
#ifndef IAP_FLASH_ACCESS_TIME
#define IAP_FLASH_ACCESS_TIME kFlash_IAP_OneSystemClockTime
#endif

static uint32_t s_PageBuf[FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES / sizeof(uint32_t)];
static uint32_t s_IapFlashPageNum = (FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES);
//static uint32_t s_IapFlashPage = ((FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES) * IAP_FLASH_SECTOR);

unsigned int initADDR = IAP_FLASH_SECTOR*1024;



unsigned char readCharFlashTest(unsigned int addr)
{
    return *((unsigned char*)(addr+initADDR));
}

unsigned char writeCharTest(unsigned char value, unsigned int addr)
{
    IAP_PrepareSectorForWrite(IAP_FLASH_SECTOR, IAP_FLASH_SECTOR);
    IAP_EraseSector(IAP_FLASH_SECTOR, IAP_FLASH_SECTOR, SystemCoreClock);
    for (int i = 0; i < FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES; i++)
        *(((unsigned char *)(&s_PageBuf[0])) + i) = 2*i +1;
    for (int i = 0; i < s_IapFlashPageNum; i++)
    {
        IAP_PrepareSectorForWrite(IAP_FLASH_SECTOR, IAP_FLASH_SECTOR);
        //IAP_CopyRamToFlash((s_IapFlashPage + i) * FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, &s_PageBuf[0],
        //                            FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
        IAP_CopyRamToFlash(initADDR, &s_PageBuf[0], 64, SystemCoreClock);
    }
    return FlashFail;
}
*/

myFlash *newFlash(unsigned int size)
{
    myFlash *ptrAux = (myFlash*)malloc(sizeof(myFlash));
    //ptrAux->_data = (unsigned char*)malloc(sizeof(unsigned char)*size);
    //unsigned char ptrData[size];
    ptrAux->_FlashPage = (FLASH_SIZE - size) / 64;
    ptrAux->_initAddr  = ptrAux->_FlashPage * 64;
    ptrAux->_FlashSector = ptrAux->_FlashPage / 16;
    ptrAux->_size = size;
    /*for (int i = 0; i < size; i++)
        ptrAux->_data[i] = iap_readChar(ptrAux,i);*/
    /*for (int i = 0; i < size; i++)
        *(((unsigned char *)(&ptrAux->_data) + i)) = iap_readChar(ptrAux,i);*/
    return ptrAux;
}
unsigned char iap_readChar(myFlash *ptrFlash, unsigned int addr)
{
    return *((unsigned char*)(addr + ptrFlash->_initAddr));
}
unsigned short iap_readShort(myFlash *ptrFlash, unsigned int addr)
{
    unsigned short value = 0;
    for (unsigned char i = 0; i < sizeof(unsigned short); i++)
        value = (*((unsigned char*)(i+addr+ptrFlash->_initAddr))) << (i*8);
    return value;
}
unsigned int iap_readInt(myFlash *ptrFlash, unsigned int addr)
{
    unsigned int value = 0;
    for (unsigned char i = 0; i < sizeof(unsigned int); i++)
        value = (*((unsigned char*)(i+addr+ptrFlash->_initAddr))) << (i*8);
    return value;
}
unsigned char iap_writeChar(myFlash *ptrFlash, unsigned char value, unsigned int addr)
{
    //*((unsigned char *)(&ptrFlash->_data)+addr) = value;
    return FlashOK;
}
unsigned char iap_writeShort(myFlash *ptrFlash, unsigned short value, unsigned int addr)
{
    for (unsigned char i = 0; i < sizeof(unsigned short); i++)
        iap_writeChar(ptrFlash, (unsigned char) (value>>(8*i)), (addr+i));
    return FlashOK;
}
unsigned char iap_writeInt(myFlash *ptrFlash, unsigned int value, unsigned int addr)
{
    for (unsigned char i = 0; i < sizeof(unsigned int); i++)
        iap_writeChar(ptrFlash, (unsigned char) (value>>(8*i)), (addr+i));
    return FlashOK;
}
unsigned char iap_commit(myFlash *ptrFlash)
{
    //Prepara o setor para escrite (1024 bytes)
    IAP_PrepareSectorForWrite(ptrFlash->_FlashSector, FLASH_SIZE_sectors);
    //Limpa as paginas preparadas para escrita (64 bytes * n)
    IAP_ErasePage(ptrFlash->_FlashPage, FLASH_SIZE_pages, SystemCoreClock);
    long unsigned int PageBufAux[ptrFlash->_size / sizeof(unsigned int)];
    /*for (int i = 0; i < ptrFlash->_size; i++)
        *(((unsigned char *)(&PageBufAux[0])) + i) = *(((unsigned char *)(&ptrFlash->_data) + i));*/
    /*for (int i = 0; i < ptrFlash->_size; i++)
        *(((unsigned char *)(&PageBufAux[0])) + i) = ptrFlash->_data[i];*/
    IAP_PrepareSectorForWrite(ptrFlash->_FlashSector, FLASH_SIZE_sectors);
    IAP_CopyRamToFlash(ptrFlash->_initAddr, &PageBufAux[0], ptrFlash->_size, SystemCoreClock);
    return FlashOK;
}

