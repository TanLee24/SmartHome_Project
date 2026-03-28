#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LIGHT_SENSOR GPIO_NUM_27 // P2
#define RGB_LED GPIO_NUM_32 // P0
#define PIR_SENSOR GPIO_NUM_2 // P3

extern float glob_temperature;
extern float glob_humidity;
extern int glob_light;
extern bool glob_human_detected;

// P0 = 32
// P1 = 33
// P14.P15 = 19
// P10.P13 = 26
// P3.P6 = 2
// P2 = 27
// P16.P12 = 5 // Not for sensor

#endif