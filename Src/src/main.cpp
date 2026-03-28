#include "global.h"

// Include Tasks
#include "led_blinky.h"
#include "light_control_led.h"
#include "temp_humi.h"
#include "luminous_intensity.h"
#include "lcd_display.h"

void setup() 
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    // xTaskCreate(ledBlinky, "Task LED Blinky", 4096, NULL, 1, NULL);
    // xTaskCreate(lightControlsLed, "Task Light Controls LED", 4096, NULL, 1, NULL);
    xTaskCreate(temp_humi, "Task Temperature Humidity", 4096, NULL, 1, NULL);
    xTaskCreate(luminousIntensity, "Task Luminous Intensity", 4096, NULL, 1, NULL);
    xTaskCreate(lcdDisplay, "Task LCD Display", 4096, NULL, 1, NULL);
}

void loop() {}