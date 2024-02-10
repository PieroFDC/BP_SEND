#include "main.hpp"

#define MODE_SWITCH PB9
#define START_STOP_SWITCH PB8
#define RETUR_NHOME_SWITCH PB7

void timerCallbackNRF() {
    bool rslt = radioTX(dataToNRFStruct);
}

void timerCallbackSerial() {
  Serial.print("<");
  Serial.print(dataToPcStruct.lat, 7);
  Serial.print(",");
  Serial.print(dataToPcStruct.lon, 7);
  Serial.print(",");
  Serial.print(dataToPcStruct.heading, 2);
  Serial.print(",");
  Serial.print(dataToPcStruct.velocity, 2);
  Serial.print(",");
  Serial.print(dataToPcStruct.numWaypoints);
  Serial.print(",");
  Serial.print(dataToPcStruct.navMode);
  Serial.print(",");
  Serial.print(dataToPcStruct.startNav);
  Serial.print(",");
  Serial.print(dataToPcStruct.returnHome);
  Serial.print(",");
  Serial.print(dataToPcStruct.battery, 2);
  Serial.print(",");
  Serial.print(dataToPcStruct.sonic);
  Serial.print(",");
  Serial.print(dataToPcStruct.calibration);
  Serial.println(">");
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

  dataToPcStruct.lat = dataFromNRFStruct.lat;
  dataToPcStruct.lon = dataFromNRFStruct.lon;
  dataToPcStruct.heading = dataFromNRFStruct.heading;
  dataToPcStruct.velocity = dataFromNRFStruct.velocity;
  dataToPcStruct.numWaypoints = dataFromNRFStruct.numWaypoints;
  dataToPcStruct.navMode = (dataToNRFStruct.mode) ? 'A' : 'M';
  dataToPcStruct.startNav = dataToNRFStruct.startStop;
  dataToPcStruct.returnHome = dataToNRFStruct.returnHome;
  dataToPcStruct.battery = dataFromNRFStruct.battery;
  dataToPcStruct.sonic = dataFromNRFStruct.sonic;
  dataToPcStruct.calibration = dataFromNRFStruct.calibration;

  delayMicroseconds(500);
}