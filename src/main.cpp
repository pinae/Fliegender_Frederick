#include <Arduino.h>
#include <Wire.h>
#include <HID-Project.h>
#include "i2cMCP.h"
#include "keypresses.h"

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
#define FLAPS_I2C_PIN_0 9
#define FLAPS_I2C_PIN_1 10
#define FLAPS_I2C_PIN_2 8
#define FLAPS_I2C_PIN_3 11
#define LANDING_GEAR_I2C_PIN_0 12
#define LANDING_GEAR_I2C_PIN_1 13
#define ENCODER_A_PIN A1
#define ENCODER_B_PIN A0
#define ENCODER_SWITCH_PIN A2
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
unsigned char flapsHistory[4] = {0, 0, 0, 0};
unsigned int flapsState = 0;
unsigned char landingGearHistory[2] = {0, 0};
bool landingGearState = false;
uint8_t encoderState = 0;
uint8_t switchState = 0;
unsigned long printTimerLastTrigger = 0;
unsigned long loopTimer = 0;
unsigned long debounceTimer = 0;

bool debounce(unsigned char* history, bool* state, bool reading) {
  *history = ((*history) << 1) | (unsigned char) reading;
  switch (*history) {
    case 0b00000000: *state = false; return false;
    case 0b11111111: *state = true; return true;
    default: return *state;
  }
}

void detectSwitchToggle(unsigned char* history, bool* state, bool reading, 
                        void (*callback)(bool state)) {
  bool oldState = *state;
  if (oldState != debounce(history, state, reading)) {
    callback(*state);
  }
}

unsigned int getFlapsState() {
  bool state0 = (flapsState == 0);
  debounce(&flapsHistory[0], &state0, i2cDigitalRead((unsigned short) FLAPS_I2C_PIN_0));
  bool state1 = (flapsState == 1);
  debounce(&flapsHistory[1], &state1, i2cDigitalRead((unsigned short) FLAPS_I2C_PIN_1));
  bool state2 = (flapsState == 2);
  debounce(&flapsHistory[2], &state2, i2cDigitalRead((unsigned short) FLAPS_I2C_PIN_2));
  bool state3 = (flapsState == 3);
  debounce(&flapsHistory[3], &state3, i2cDigitalRead((unsigned short) FLAPS_I2C_PIN_3));
  unsigned char state = (state0 << 3) | (state1 << 1) | (state2 << 2) | state3;
  switch (state) {
    case 0b00001000: flapsState = 0; break;
    case 0b00000100: flapsState = 1; break;
    case 0b00000010: flapsState = 2; break;
    case 0b00000001: flapsState = 3; break;
  }
  return flapsState;
}

bool getLandingGearState() {
  bool state0 = landingGearState;
  bool state1 = !landingGearState;
  debounce(&landingGearHistory[0], &state0, 
           i2cDigitalRead((unsigned short) LANDING_GEAR_I2C_PIN_0));
  debounce(&landingGearHistory[1], &state1, 
           i2cDigitalRead((unsigned short) LANDING_GEAR_I2C_PIN_1));
  unsigned char state = (state0 << 1) | state1;
  switch (state) {
    case 0b00000010: landingGearState = false; break;
    case 0b00000001: landingGearState = true; break;
  }
  return landingGearState;
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
  debounceTimer = millis();
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
      else              { encoderState = STATE_LOCKED; trimWheelStepRight(); }; 
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
      else              { encoderState = STATE_LOCKED; trimWheelStepLeft(); }; 
      break;
    case STATE_UNDECIDED:
      if (a && b)       { encoderState = STATE_UNDECIDED; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
  }
  if (!switchState && s) trimWheelSwitchPress();
  switchState = s;

  if (millis() > debounceTimer + 25) {
    debounceTimer = millis();
    detectSwitchToggle(&switchA0History, &switchA0State, 
                       digitalRead(SWITCH_A_0_PIN), switchA0);
    detectSwitchToggle(&switchA1History, &switchA1State, 
                       digitalRead(SWITCH_A_1_PIN), switchA1);
    detectSwitchToggle(&switchA2History, &switchA2State, 
                       digitalRead(SWITCH_A_2_PIN), switchA2);
    detectSwitchToggle(&switchB0History, &switchB0State, 
                       digitalRead(SWITCH_B_0_PIN), switchB0);
    detectSwitchToggle(&switchB1History, &switchB1State, 
                       digitalRead(SWITCH_B_1_PIN), switchB1);
    detectSwitchToggle(&switchB2History, &switchB2State, 
                       digitalRead(SWITCH_B_2_PIN), switchB2);
    detectSwitchToggle(&switchC0History, &switchC0State, 
                       i2cDigitalRead((unsigned short) SWITCH_C_0_I2C_PIN),
                       switchC0);
    detectSwitchToggle(&switchC1History, &switchC1State, 
                       !i2cDigitalRead((unsigned short) SWITCH_C_1_I2C_PIN),
                       switchC1);
    detectSwitchToggle(&switchD0History, &switchD0State, 
                       i2cDigitalRead((unsigned short) SWITCH_D_0_I2C_PIN),
                       switchD0);
    detectSwitchToggle(&switchD1History, &switchD1State, 
                       i2cDigitalRead((unsigned short) SWITCH_D_1_I2C_PIN),
                       switchD1);
    getFlapsState();
    getLandingGearState();
  }

  if (millis() > printTimerLastTrigger + 1000) {
    printTimerLastTrigger = millis();
    Serial.print("Loop time: "); Serial.println(millis() - loopTimer);
    Serial.print(a); Serial.print(" "); Serial.print(b); Serial.print(" "); Serial.println(s);
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
    Serial.print("Flaps state: "); Serial.println(flapsState);
    Serial.print("Landing gear state: "); Serial.println(landingGearState);
  }

  loopTimer = millis();
}