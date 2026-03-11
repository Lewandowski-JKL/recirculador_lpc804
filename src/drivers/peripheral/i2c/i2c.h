#ifndef I2C_H
#define I2C_H

enum I2cStat{
    i2cStat_Fail = -1,
    i2cStat_TimeOut,
    i2cStat_OK
};
enum I2cMode{
    i2cModeSlave=1,
    i2cModeMaster
};
void i2cBegin(unsigned char addr, unsigned char sda, unsigned char scl, unsigned int baudRate, enum I2cMode mode);

void i2cBeginSlave(unsigned char addr, unsigned char sda, unsigned char scl, unsigned int baudRate);
int i2cRead_slave(unsigned char buffer[]);
int i2cSend_slave(unsigned char buffer[], int size);

void i2cBeginMaster(unsigned char sda, unsigned char scl, unsigned int baudRate);
int i2cRead_master(unsigned char addr, unsigned char buffer[]);
int i2cSend_master(unsigned char addr, unsigned char *message, int size);
int i2cReadLenght_master(unsigned char addr, unsigned char buffer[], int len);

#endif