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

#endif //__MAIN_HPP