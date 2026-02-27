#ifndef REGISTERS_ADDR_H
#define REGISTERS_ADDR_H
#define WIFI_ADDR  0x01//0x01
#define MY_ADDR  0x02//0x01

//Codigo de equipamento
#define CODE_RECIRCULADOR   0x10
#define CODE_SOLAR_G2       0x11

//Functions code ModBus
#define GET_BOOLEAN_REGISTERS           0x01
#define GET_NUMERIC_REGISTERS           0x03
#define SET_BOOLEAN_REGISTER            0x05
#define SET_SINGLE_REGISTER             0x06
#define SET_MULTIPLE_BOOLEAN_REGISTERS  0x0F
#define SET_MULTIPLE_REGISTERS          0x10


const unsigned char typeBool = sizeof(bool);
const unsigned char typeChar = sizeof(char);
const unsigned char typeShort = sizeof(short);
const unsigned char typeInt = sizeof(int);
const unsigned char typeFloat = sizeof(float);

#endif