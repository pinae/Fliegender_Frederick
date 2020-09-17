#include <Arduino.h>
#include <Wire.h>
#include "i2cMCP.h"

#ifndef I2C_MPC_GLOBALS
#define I2C_MPC_GLOBALS
unsigned char i2cOutput[2] = {'\x00', '\x00'};
#endif

void printBinary(char data) {
  for (unsigned short i = 0; i < 8; i++) {
    Serial.print((data >> (7 - i)) & 1);
  }
}

void printAdr(byte adr) {
  if (adr < 16) Serial.print("0");
  Serial.print(adr, HEX);
}

void i2cScanDevices() {
  unsigned char error, adr;
  for(adr = '\x03'; adr <= '\x7f'; adr++ ) {
    if((adr & 0b11111110) == 0b00000100) continue;
    if((adr & 0b11111110) == 0b00000110) continue;
    if((adr & 0b11111000) == 0b00001000) continue;
    if((adr & 0b11111000) == 0b11110000) continue;
    if((adr & 0b11111000) == 0b11111000) continue;
    Wire.beginTransmission(adr);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("i2c device found at address 0x");
      printAdr(adr);
      Serial.println("!");
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      printAdr(adr);
      Serial.println("!");
    }    
  }
  Serial.println("i2c scan completed.");
}

void i2cDigitalWrite(unsigned short pinNo, bool value) {
  Wire.beginTransmission(GPIO_EXTENDER_ADDR);
  unsigned short bank;
  if (pinNo < 8) {
    Wire.write('\x14');
    bank = 0;
  } else {
    Wire.write('\x15');
    bank = 1;
  }
  unsigned short pinNo8 = (pinNo & 0b00000111);
  unsigned char pm = 1 << pinNo8;
  if (value) {
    i2cOutput[bank] |= pm;
  } else {
    i2cOutput[bank] &= ('\xff' ^ pm);
  }
  Wire.write(i2cOutput[bank]);
  Wire.endTransmission();
}

unsigned char i2cDigitalRead(bool bankA) {
  Wire.beginTransmission(GPIO_EXTENDER_ADDR);
  if (bankA) {
    Wire.write('\x12');
  } else {
    Wire.write('\x13');
  }
  Wire.endTransmission();
  Wire.requestFrom(GPIO_EXTENDER_ADDR, 1);
  while (!Wire.available()) {};
  return Wire.read();
}

bool i2cDigitalRead(unsigned short pinNo) {
  bool bankA = (pinNo >> 3) == 0;
  unsigned char mask = 0b00000001 << (pinNo & 0b00000111);
  return i2cDigitalRead(bankA) & mask;
}