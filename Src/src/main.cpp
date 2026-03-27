#include "global.h"

// Include Tasks
#include "led_blinky.h"
#include "light_control_led.h"
#include "temp_humi.h"

void setup() 
{
    Serial.begin(115200);
    // xTaskCreate(ledBlinky, "Task LED Blinky", 4096, NULL, 2, NULL);
    xTaskCreate(lightControlsLed, "Task Light Controls LED", 4096, NULL, 2, NULL);
    xTaskCreate(temp_humi, "Task Temperature Humidity", 4096, NULL, 1, NULL);
}

void loop() {}