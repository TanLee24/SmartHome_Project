#ifndef __DOOR_CONTROL_H__
#define __DOOR_CONTROL_H__

#include "global.h"
#include <ESP32Servo.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

void doorControl(void *pvParameters);

#endif