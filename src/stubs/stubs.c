#include <stdint.h>
#include <stdbool.h>

__attribute__((weak)) void setClock30M(void) {}
__attribute__((weak)) void portBegin(void) {}
__attribute__((weak)) void setOutputPin(uint8_t ch, uint8_t pin) {}
__attribute__((weak)) void clearPin(uint8_t ch, uint8_t pin) {}
__attribute__((weak)) void setInputPin(uint8_t ch, uint8_t pin) {}
__attribute__((weak)) void setPin(uint8_t ch, uint8_t pin) {}
__attribute__((weak)) bool readInputPin(uint8_t ch, uint8_t pin) { return false; }

__attribute__((weak)) void SysTickBeginISR(uint32_t hz) {}
__attribute__((weak)) uint32_t SysTickGetTicks(void) { return 0; }
__attribute__((weak)) uint32_t SysTickGetTime_ms(void) { return 0; }

__attribute__((weak)) void myi2cBegin(void) {}
__attribute__((weak)) int  sendI2C_master(uint8_t addr, const uint8_t *buf, uint16_t len) { return -1; }
__attribute__((weak)) int  readLenI2C_master(uint8_t addr, uint8_t *buf, uint16_t len) { return -1; }

__attribute__((weak)) void newThermistor(void) {}
__attribute__((weak)) float convert_mV_to_celcius(float mv) { return 0.0f; }

__attribute__((weak)) void initRegisters(void) {}
__attribute__((weak)) bool     getBoolWithAddr(uint16_t addr) { return false; }
__attribute__((weak)) void     setBoolValue(uint16_t addr, bool v) {}
__attribute__((weak)) int32_t  getIntWithAddr(uint16_t addr) { return 0; }
__attribute__((weak)) float    getFloatWithAddr(uint16_t addr) { return 0.0f; }
__attribute__((weak)) void     setFloatValue(uint16_t addr, float v) {}
__attribute__((weak)) int compareShortRegister(void) { return 0; }
__attribute__((weak)) int compareIntRegister(void) { return 0; }
__attribute__((weak)) int compareFloatRegister(void) { return 0; }
__attribute__((weak)) int compareBoolRegister(void) { return 0; }
__attribute__((weak)) volatile uint16_t *ShortRegisters;
__attribute__((weak)) volatile int32_t  *IntRegisters;
__attribute__((weak)) volatile float    *FloatRegisters;
__attribute__((weak)) volatile uint16_t *LastShortRegisters;
__attribute__((weak)) volatile int32_t  *LastIntRegisters;
__attribute__((weak)) volatile float    *LastFloatRegisters;
__attribute__((weak)) volatile bool     *LastBoolRegisters;

__attribute__((weak)) void addNewMessage(uint8_t t, uint16_t a, uint16_t b) {}
__attribute__((weak)) void deleteMessage(uint8_t t) {}
__attribute__((weak)) uint16_t CHECK_CRC16(const uint8_t *buf, uint16_t len) { return 0; }

__attribute__((weak)) void newExternInterrupt(uint8_t ch, uint8_t pin, void (*cb)(void)) {}

__attribute__((weak)) int IAP_PrepareSectorForWrite(uint32_t a, uint32_t b) { return -1; }
__attribute__((weak)) int IAP_ErasePage(uint32_t addr, uint32_t bytes) { return -1; }
__attribute__((weak)) int IAP_CopyRamToFlash(uint32_t dst, const void *src, uint32_t bytes) { return -1; }
