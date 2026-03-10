#ifndef REGISTERS_MANAGER_H
#define REGISTERS_MANAGER_H

#include <stdlib.h>
#include <stdbool.h>
#include "board_defs.h"



void reg_Begin();
void reg_write_bool(bool value, unsigned int addr);
void reg_write_bool_vet(bool *vet, unsigned int size, unsigned int addr);
void reg_write_short(short value, unsigned int addr);
void reg_write_short_vet(short *vet, unsigned int size, unsigned int addr);
void reg_write_int(int value, unsigned int addr);
void reg_write_int_vet(int *vet, unsigned int size, unsigned int addr);
#ifndef __NO_FLOAT__
void reg_write_float(float value, unsigned int addr);
void reg_write_float_vet(float *vet, unsigned int size, unsigned int addr);
#endif
void reg_write(void *val, unsigned int addr);
void reg_write_vet(void *vet, unsigned int nRegs, unsigned int addr);

/***********************************
 * Read
 ***********************************/
bool reg_read_bool(unsigned int addr);
void reg_read_bool_vet(bool *vet, unsigned int size, unsigned int addr);
short reg_read_short(unsigned int addr);
void reg_read_short_vet(short *vet, unsigned int size, unsigned int addr);
int reg_read_int(unsigned int addr);
void reg_read_int_vet(int *vet, unsigned int size, unsigned int addr);
#ifndef __NO_FLOAT__
float reg_read_float(unsigned int addr);
void reg_read_float_vet(float *vet, unsigned int size, unsigned int addr);
#endif
void reg_read(void *ptr, unsigned int addr);
void reg_read_vet(void *vet, unsigned int nRegs, unsigned int addr);

#endif 
