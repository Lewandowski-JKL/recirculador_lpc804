#include <stdlib.h>
#include "registers_manager.h"
#include "board.h"
#include "app_config.h"

#define __SHRT_MIN__    (0xFFFF-__SHRT_MAX__)
#define __INT_MIN__     (0xFFFFFFFF-__INT_MAX__)



// #define regMapBoolOffset 0x0448
// enum regMapBool
// {
//     regMapBomba = regMapBoolOffset+1,
//     regMapBotoeira,
//     regMapSolenoideA,
//     regMapSolenoideB,
//     regMapTotalBool = regMapSolenoideB-regMapBoolOffset,
// };
// /* void setShortWithAddr(short value, short addr);
// short getShortWithAddr(short addr);*/
// #define regMapShortOffset 0x0BB8
// enum regMapShort
// {
//     regMapModelo = regMapShortOffset+1,
//     regMapFirmwareVersionInit,
//     regMapFirmwareVersionEnd = regMapFirmwareVersionInit+2,
//     regMapEnderecoMacInit,
//     regMapEnderecoMacEnd = regMapEnderecoMacInit+2,
//     regMapNomeDaRedeInit,
//     regMapNomeDaRedeEnd = regMapNomeDaRedeInit+15,
//     regMapSenhaDaRedeInit,
//     regMapSenhaDaRedeEnd = regMapSenhaDaRedeInit+15,
//     regMapTotalShort = regMapSenhaDaRedeEnd-regMapShortOffset,
// };
// #define regMapIntOffset 0x13EB
// enum regMapInt
// {
//     regMapTimestamp = regMapIntOffset+1,
//     regMapAgendamentosInit,
//     regMapAgendamentosEnd = regMapAgendamentosInit+9,
//     regMapTotalInt = regMapAgendamentosEnd - regMapIntOffset,
// };
// #define regMapFloatOffset 0x1BBB
// enum regMapFloat
// {
//     regMapFluxo = regMapFloatOffset+1,
//     regMapTemperaturaMedida,
//     regMapLitrosTotais,
//     regMapTemperaturaRef,
//     regMapCalibFluxo,
//     regMapCalibTemp,
//     regMapTotalFloat = (regMapCalibTemp-regMapFloatOffset)
// };
// #define NumTotalRegisters  (regMapTotalBool + regMapTotalShort + regMapTotalInt + regMapTotalFloat)


void reg_Begin()
{
    

}