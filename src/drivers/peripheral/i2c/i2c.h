#ifndef I2C_H
#define I2C_H

typedef struct __attribute__((packed)) i2c_modbus_s
{
    unsigned char code;
    short RegAddr;
    short nReg;
    char bytes;
    unsigned char *ptrMessage;
    unsigned short crc;
    //////////////////
    unsigned char addr;
    short size_head;
    short size_message;
    //short size_reply;
    //void (*ptrProcess)(i2c_modbus_s *message);
}i2c_modbus_s;

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
int i2cReadClearBuffer_master(unsigned char addr);

int i2cSend_master_modbus(i2c_modbus_s *ptrMessage);
//int i2cRead_lenght_master_modbus(i2c_modbus_s *ptrMessage, unsigned char buffer[]);

#endif