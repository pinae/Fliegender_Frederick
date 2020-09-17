#include <Arduino.h>
#include <Wire.h>
#include <HID-Project.h>
#include "i2cMCP.h"

#define SWITCH_A_0_PIN 9
#define SWITCH_A_1_PIN 4
#define SWITCH_A_2_PIN 6
#define SWITCH_B_0_PIN 8
#define SWITCH_B_1_PIN 7
#define SWITCH_B_2_PIN 5
#define ENCODER_A_PIN 18
#define ENCODER_B_PIN 17
#define ENCODER_SWITCH_PIN 16
#define STATE_LOCKED 0
#define STATE_TURN_RIGHT_START 1
#define STATE_TURN_RIGHT_MIDDLE 2
#define STATE_TURN_RIGHT_END 3
#define STATE_TURN_LEFT_START 4
#define STATE_TURN_LEFT_MIDDLE 5
#define STATE_TURN_LEFT_END 6
#define STATE_UNDECIDED 7

uint8_t encoderState = 0;
uint8_t switchState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_A_0_PIN, INPUT);
  pinMode(SWITCH_A_1_PIN, INPUT);
  pinMode(SWITCH_A_2_PIN, INPUT);
  pinMode(SWITCH_B_0_PIN, INPUT);
  pinMode(SWITCH_B_1_PIN, INPUT);
  pinMode(SWITCH_B_2_PIN, INPUT);
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(ENCODER_SWITCH_PIN, INPUT);
  encoderState = 0;
  switchState = 0;
  Consumer.begin();
  while(!Serial) { }  // Wait for Serial to start
  Serial.println("Serial ready.");
  Wire.begin();
  Wire.beginTransmission(GPIO_EXTENDER_ADDR);
  Wire.write('\x00');
  Wire.write((unsigned char) 0b11111111);
  Wire.endTransmission();
  Wire.beginTransmission(GPIO_EXTENDER_ADDR);
  Wire.write('\x01');
  Wire.write((unsigned char) 0b11111111);
  Wire.endTransmission();
  Serial.println("Setup complete.");
  i2cScanDevices();
}

void stepRight() {
  Serial.println(" -->");
  //Consumer.write(MEDIA_VOL_UP);
}

void stepLeft() {
  Serial.println("<-- ");
  //Consumer.write(MEDIA_VOL_DOWN);
}

void switchPress() {
  Serial.println("- SWITCH -");
  //Consumer.write(MEDIA_VOL_MUTE);
}

void loop() {
  uint8_t a = digitalRead(ENCODER_A_PIN);
  uint8_t b = digitalRead(ENCODER_B_PIN);
  uint8_t s = digitalRead(ENCODER_SWITCH_PIN);
  switch (encoderState) {
    case STATE_LOCKED: 
      if (a && b)       { encoderState = STATE_UNDECIDED; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_RIGHT_START: 
      if (a && b)       { encoderState = STATE_TURN_RIGHT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_RIGHT_MIDDLE:
    case STATE_TURN_RIGHT_END:
      if (a && b)       { encoderState = STATE_TURN_RIGHT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; stepRight(); }; 
      break;
    case STATE_TURN_LEFT_START: 
      if (a && b)       { encoderState = STATE_TURN_LEFT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_LEFT_MIDDLE: 
    case STATE_TURN_LEFT_END: 
      if (a && b)       { encoderState = STATE_TURN_LEFT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; stepLeft(); }; 
      break;
    case STATE_UNDECIDED:
      if (a && b)       { encoderState = STATE_UNDECIDED; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
  }
  if (!switchState && s) switchPress();
  switchState = s;

  Serial.print("Switch A0: "); Serial.print(digitalRead(SWITCH_A_0_PIN));
  Serial.print(" A1: "); Serial.print(digitalRead(SWITCH_A_1_PIN));
  Serial.print(" A2: "); Serial.println(digitalRead(SWITCH_A_2_PIN));
  Serial.print("Switch B0: "); Serial.print(digitalRead(SWITCH_B_0_PIN));
  Serial.print(" B1: "); Serial.print(digitalRead(SWITCH_B_1_PIN));
  Serial.print(" B2: "); Serial.println(digitalRead(SWITCH_B_2_PIN));
  bool c0 = i2cDigitalRead((unsigned short) 0);
  Serial.print("Switch C0: "); Serial.println(c0);
  bool c1 = i2cDigitalRead((unsigned short) 3);
  Serial.print("Switch C1: "); Serial.println(!c1);
  bool d0 = i2cDigitalRead((unsigned short) 2);
  Serial.print("Switch D0: "); Serial.println(d0);
  bool d1 = i2cDigitalRead((unsigned short) 1);
  Serial.print("Switch D1: "); Serial.println(d1);


  Serial.print("MCP: "); 
  printBinary(i2cDigitalRead(true)); Serial.print(" ");
  printBinary(i2cDigitalRead(false)); Serial.println(".");
  delay(500);
}