#ifndef __JOYSTICK_HPP
#define __JOYSTICK_HPP

#include <Arduino.h>

#include "data.hpp"

#define JOYX PA1
#define JOYY PA2

void getJoystickMotors(dataToNRF& motors) {
  int vx = analogRead(JOYX);
  int vy = analogRead(JOYY);
  
  int pwml;
  int pwmr;

  vx = map(vx, 0, 4095, -2010, 2010);
  vy = map(vy, 0, 4095, 2010, -2010);
  
  if(vx > -120 && vx < 120) {
    vx = 0;
  }

  if(vy > -120 && vy < 120) {
    vy = 0;
  }

  vx = constrain(vx, -2000, 2000);
  vy = constrain(vy, -2000, 2000);

  pwml = vy + vx;
  pwmr = vy - vx;

  if(vx < 0) {
    if(vy < 0) {
      if(vx >= vy) {
        pwml = vy;
      } else if(vx < vy) {
        pwml = vx;
      }
    } else if(vy > 0) {
      if(-vx >= vy) {
        pwmr = -vx;
      } else if(-vx < vy) {
        pwmr = vy;
      }
    }
  } else if(vx > 0) {
    if(vy < 0) {
      if(-vx >= vy) {
        pwmr = vy;
      } else if(-vx < vy) {
        pwmr = -vx;
      }
    } else if(vy > 0) {
      if(vx >= vy) {
        pwml = vx;
      } else if(vx < vy) {
        pwml = vy;
      }
    }
  }

  motors.pwml = map(pwml, -2000, 2000, 1100, 1900);
  motors.pwmr = map(pwmr, -2000, 2000, 1100, 1900);
}

#endif //__JOYSTICK_HPP