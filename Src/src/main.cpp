#include "global.h"

// Include Tasks
#include "led_blinky.h"
#include "light_control_led.h"
#include "temp_humi.h"
#include "luminous_intensity.h"
#include "lcd_display.h"
#include "human_detect.h"
#include "door_control.h"
#include "mqtt_connect.h"

void setup() 
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    // xTaskCreate(ledBlinky, "Task LED Blinky", 4096, NULL, 1, NULL);
    // xTaskCreate(lightControlsLed, "Task Light Controls LED", 4096, NULL, 1, NULL);

    // Task to read Temperature and Humidity from DHT20
    xTaskCreate(temp_humi, "Task Temperature Humidity", 4096, NULL, 1, NULL);
    // Task to read Luminous intensity from Light sensor
    xTaskCreate(luminousIntensity, "Task Luminous Intensity", 4096, NULL, 1, NULL);
    // Task to Display Temeperature, Humidity and Luminous Intensity on LCD
    xTaskCreate(lcdDisplay, "Task LCD Display", 4096, NULL, 1, NULL);
    // Task to Detect moving objects (human) using PIR sensor
    xTaskCreate(humanDetect, "Task Detect Human", 4096, NULL, 1, NULL);
    // Task to Open door based on Password and Change Password
    xTaskCreate(doorControl, "Task Door Control", 4096, NULL, 1, NULL);
    // Task to create MQTT connect and Monitoring/Controlling
    xTaskCreate(mqttConnect, "Task MQTT Connect", 4096, NULL, 1, NULL);
}

void loop() {}