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
#define SWITCH_C_0_I2C_PIN 0
#define SWITCH_C_1_I2C_PIN 3
#define SWITCH_D_0_I2C_PIN 2
#define SWITCH_D_1_I2C_PIN 1
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

unsigned char switchA0History = 0;
bool switchA0State = false;
unsigned char switchA1History = 0;
bool switchA1State = false;
unsigned char switchA2History = 0;
bool switchA2State = false;
unsigned char switchB0History = 0;
bool switchB0State = false;
unsigned char switchB1History = 0;
bool switchB1State = false;
unsigned char switchB2History = 0;
bool switchB2State = false;
unsigned char switchC0History = 0;
bool switchC0State = false;
unsigned char switchC1History = 0;
bool switchC1State = false;
unsigned char switchD0History = 0;
bool switchD0State = false;
unsigned char switchD1History = 0;
bool switchD1State = false;
uint8_t encoderState = 0;
uint8_t switchState = 0;
unsigned long printTimerLastTrigger = 0;
unsigned long loopTimer = 0;

bool debounce(unsigned char* history, bool* state, bool reading) {
  *history = ((*history) << 1) | (unsigned char) reading;
  switch (*history) {
    case 0b00000000: *state = false; return false;
    case 0b11111111: *state = true; return true;
    default: return *state;
  }
}

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
  printTimerLastTrigger = millis();
  loopTimer = millis();
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

  debounce(&switchA0History, &switchA0State, digitalRead(SWITCH_A_0_PIN));
  debounce(&switchA1History, &switchA1State, digitalRead(SWITCH_A_1_PIN));
  debounce(&switchA2History, &switchA2State, digitalRead(SWITCH_A_2_PIN));
  debounce(&switchB0History, &switchB0State, digitalRead(SWITCH_B_0_PIN));
  debounce(&switchB1History, &switchB1State, digitalRead(SWITCH_B_1_PIN));
  debounce(&switchB2History, &switchB2State, digitalRead(SWITCH_B_2_PIN));
  debounce(&switchC0History, &switchC0State, 
           i2cDigitalRead((unsigned short) SWITCH_C_0_I2C_PIN));
  debounce(&switchC1History, &switchC1State, 
           !i2cDigitalRead((unsigned short) SWITCH_C_1_I2C_PIN));
  debounce(&switchD0History, &switchD0State, 
           i2cDigitalRead((unsigned short) SWITCH_D_0_I2C_PIN));
  debounce(&switchD1History, &switchD1State, 
           i2cDigitalRead((unsigned short) SWITCH_D_1_I2C_PIN));

  if (millis() > printTimerLastTrigger + 1000) {
    printTimerLastTrigger = millis();
    Serial.print("Loop time: "); Serial.println(millis() - loopTimer);
    Serial.print("Switch A0: "); Serial.print(switchA0State);
    Serial.print(" A1: "); Serial.print(switchA1State);
    Serial.print(" A2: "); Serial.println(switchA2State);
    Serial.print("Switch B0: "); Serial.print(switchB0State);
    Serial.print(" B1: "); Serial.print(switchB1State);
    Serial.print(" B2: "); Serial.println(switchB2State);
    Serial.print("Switch C0: "); Serial.print(switchC0State);
    Serial.print(" C1: "); Serial.println(switchC1State);
    Serial.print("Switch D0: "); Serial.print(switchD0State);
    Serial.print(" D1: "); Serial.println(switchD1State);

    Serial.print("MCP: "); 
    printBinary(i2cDigitalRead(true)); Serial.print(" ");
    printBinary(i2cDigitalRead(false)); Serial.println(".");
  }

  loopTimer = millis();
  delay(23);
}