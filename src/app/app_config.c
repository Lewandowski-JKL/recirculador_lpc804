/*
 * commomLib.cpp
 *
 *  Created on: 18 de mar. de 2024
 *      Author: jackson.lewandowski
 */
#include "app_config.h"
#include "board.h"
#include <stdlib.h>

//Extern variables
//extern SMHandle *SerialSend;
//extern SMHandle *SerialReceive;
// unsigned char TaskRunFlag;
// unsigned int DebugTaskFlag;

// #ifdef MYINTERRUPT_H
// void delayMs(unsigned int delay)
// {
//     delay_us((unsigned long int )delay*1000);
// }
// void delay_us(unsigned long int delay)
// {
//     unsigned long int delayRef = (delay*(SystemCoreClock/1000))/16611;
//     for (unsigned i = 0; i < delayRef; i++);
// }
// #endif

// short copyBoolToPChar(bool *src, short size, unsigned char *dest)
// {
//     unsigned short dim = size/8;
//     if (size%8)
//         dim++;
//     for (int i = 0; i < (dim*8); i++)
//     {
//         if (i>=size)
//             dest[i/8] = (dest[i/8] << 1);
//             else
//                 dest[i/8] = (dest[i/8] << 1) + src[i];
//     }
//     return dim;
// }
// short copyShortToPChar(short *src, short size, unsigned char *dest)
// {
//     for (int i = 0; i < (sizeof(short)*size); i++)
//         dest[i]=(unsigned char)src[i/sizeof(short)] >> (8*(i%sizeof(short)));
//     return sizeof(short)*size;
// }
// short copyIntToPChar(int *src, short size, unsigned char *dest)
// {
//     for (int i = 0; i < (sizeof(int)*size); i++)
//         dest[i]=(unsigned char)src[i/sizeof(int)] >> (8*(i%sizeof(int)));
//     return sizeof(int)*size;
// }
// short copyFloatToPChar(float *src, short size, unsigned char *dest)
// {
//     for (int i = 0; i < (sizeof(float)*size); i++)
//         dest[i]=(unsigned char)src[i/sizeof(float)] >> (8*(i%sizeof(float)));
//     return sizeof(float)*size;
// }

// short copyPCharToBool(unsigned char *src, short size, bool *dest)
// {
//     return 0;
// }
// short copyPCharToShort(unsigned char *src, short size, short *dest)
// {
//     short dim = size/sizeof(short);
//     if (size%sizeof(short))
//         dim++;
//     short valueAux= 0;
//     for (int i = 0; i < (dim*sizeof(short)); i++)
//     {
//         if (i >= size)
//             valueAux = 0;
//             else
//                 valueAux = src[i];
//         dest[i/sizeof(short)] += valueAux << (8*(i%sizeof(short)));
//     }
//     return dim;
// }
// short copyPCharToInt(unsigned char *src, short size, int *dest)
// {
//     short dim = size/sizeof(int);
//     if (size%sizeof(int))
//         dim++;
//     int valueAux= 0;
//     for (int i = 0; i < (dim*sizeof(int)); i++)
//     {
//         if (i >= size)
//             valueAux = 0;
//             else
//                 valueAux = src[i];
//         dest[i/sizeof(int)] += valueAux << (8*(i%sizeof(int)));
//     }
//     return dim;
// }
// short copyPCharToFloat(unsigned char *src, short size, float *dest)
// {
//     short dim = size/sizeof(float);
//     if (size%sizeof(float))
//         dim++;
//     int valueAux = 0;
//     for (int i = 0; i < (dim*sizeof(float)); i++)
//     {
//         if (i >= size)
//             valueAux = 0;
//             else
//                 valueAux = src[i];
//         dest[i/sizeof(float)] += valueAux << (8*(i%sizeof(float)));
//     }
//     return dim;
// }

