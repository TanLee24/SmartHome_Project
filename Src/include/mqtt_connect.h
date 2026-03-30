#ifndef __MQTT_CONNECT_H__
#define __MQTT_CONNECT_H__

#include "global.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

void mqttConnect(void *pvParameters);

#endif