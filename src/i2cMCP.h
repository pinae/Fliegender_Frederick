#include <Arduino.h>

#define GPIO_EXTENDER_ADDR '\x20'

void printBinary(char data);
void printAdr(byte adr);
void i2cScanDevices();
void i2cDigitalWrite(unsigned short pinNo, bool value);
unsigned char i2cDigitalRead(bool bankA);
bool i2cDigitalRead(unsigned short pinNo);