#ifndef  MYI2C_H
#define MYI2C_H


enum myI2cStat{
    i2cStat_Fail = -1,
    i2cStat_TimeOut,
    i2cStat_OK
};
//#define MASTER
#ifndef MASTER 

void i2cBeginSlave(unsigned char addr, unsigned char sda, unsigned char scl, unsigned int baudRate);
int readI2c_slave(unsigned char buffer[]);
int sendI2c_slave(unsigned char buffer[], unsigned char size);
#else
void i2cBeginMaster(unsigned char sda, unsigned char scl, unsigned int baudRate);

//int readI2C(i2cMessage *ptrI2C);
int sendI2C_master(unsigned char addr, unsigned char *message, unsigned char size);
int readLenI2C_master(unsigned char buffer[], unsigned char len);
#endif
#endif
