#ifndef __MAIN_HPP
#define __MAIN_HPP

#include <Arduino.h>
#include <HardwareTimer.h>
#include "data.hpp"
#include "joystick.hpp"
#include "nrf24com.hpp"
#include "utils.hpp"

HardwareTimer *timer_nrf = new HardwareTimer(TIM3);
HardwareTimer *timer_serial = new HardwareTimer(TIM4);

bool newDataToPC = false;
bool newDataToNRF = false;

void sendDataToPC() {
  if(newDataToPC) {
    newDataToPC = false;

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
}

void sendDataToNRF() {
  if(newDataToNRF) {
    newDataToNRF = false;
    bool rslt = radioTX(dataToNRFStruct);
  }
}

#endif //__MAIN_HPP