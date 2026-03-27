#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// P0 = 32
// P1 = 33
// P14.P15 = 19
// P10.P13 = 26
// P3.P6 = 2
// P2 = 27
// P16.P12 = 5 // Not for sensor


extern float glob_temperature;
extern float glob_humidity;

#endif