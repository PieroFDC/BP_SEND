#include "main.hpp"

#define MODE_SWITCH PB9
#define START_STOP_SWITCH PB8
#define RETUR_NHOME_SWITCH PB7

void timerCallbackNRF() {
  newDataToNRF = true;
}

void timerCallbackSerial() {
  newDataToPC = true;
}

void setup() {
  Serial.begin(2000000);
  analogReadResolution(12);

  radioSetup();

  pinMode(MODE_SWITCH, INPUT_PULLUP);
  pinMode(START_STOP_SWITCH, INPUT_PULLUP);
  pinMode(RETUR_NHOME_SWITCH, INPUT_PULLUP);


  timer_nrf->setOverflow(13, HERTZ_FORMAT);
  timer_nrf->attachInterrupt(timerCallbackNRF);
  timer_nrf->resume();

  timer_serial->setOverflow(5, HERTZ_FORMAT);
  timer_serial->attachInterrupt(timerCallbackSerial);
  timer_serial->resume();
}

void loop() {
  getJoystickMotors(dataToNRFStruct);

  dataToNRFStruct.mode = digitalRead(MODE_SWITCH);
  dataToNRFStruct.startStop = digitalRead(START_STOP_SWITCH);
  dataToNRFStruct.returnHome = digitalRead(RETUR_NHOME_SWITCH);

  getRadioData();
  sendDataToNRF();
  sendDataToPC();

  delayMicroseconds(500);
}