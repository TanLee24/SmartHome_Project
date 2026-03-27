#ifndef __LIGHT_CONTROL_LED_H__
#define __LIGHT_CONTROL_LED_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT_SENSOR_PIN GPIO_NUM_33
#define FOUR_LED_PIN GPIO_NUM_32

extern Adafruit_NeoPixel pixels3;

// Task to monitor light and affect LED: dark -> turn on LED, bright -> turn off LED
void lightControlsLed(void *pvParameters);

#endif