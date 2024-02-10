#ifndef __DATA_HPP
#define __DATA_HPP

#include <Arduino.h>

struct dataToPc {
  double lat;
  double lon;
  double heading;
  double velocity;
  int numWaypoints;
  char navMode;
  bool startNav;
  bool returnHome;
  double battery;
  bool sonic;
  int calibration;
};

struct dataToNRF {
  bool mode;
  int pwml;
  int pwmr;
  bool startStop;
  bool returnHome;
};

struct dataFromNRF {
  double lat;
  double lon;
  double heading;
  double velocity;
  int numWaypoints;
  double battery;
  bool sonic;
  int calibration;
};

#endif //__DATA_HPP