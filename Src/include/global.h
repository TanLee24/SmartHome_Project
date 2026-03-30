#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LIGHT_SENSOR GPIO_NUM_33 // P2
#define RGB_PIN GPIO_NUM_32 // P0
#define NUMPIXELS 4
#define PIR_PIN GPIO_NUM_2 // P3
#define IR_PIN 27 // P1
#define SERVO_PIN 5 // P16
#define SINGLE_LED_PIN GPIO_NUM_19 // P14

#define FAN_PIN GPIO_NUM_26 // P10
#define FAN_PWM_CHANNEL 0
#define FAN_PWM_FREQ 5000
#define FAN_PWM_RES 8 // Resolution 8-bit (0-255)

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