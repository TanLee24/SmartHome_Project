#include "human_detect.h"

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_LED, NEO_GRB + NEO_KHZ800);

void humanDetect(void *pvParameters)
{
    pinMode(PIR_SENSOR, INPUT);

    pixels.begin();
    pixels.clear();
    pixels.show();

    while (1) 
    {
        // 1 = Detected, 0 = Undetected
        bool is_detected = digitalRead(PIR_SENSOR);
        
        glob_human_detected = is_detected;

        if (is_detected) 
        {
            for(int i = 0; i < NUMPIXELS; i++) 
            {
                pixels.setPixelColor(i, pixels.Color(255, 0, 0));
            }
            Serial.println("PIR: Human detected!");
        } 
        else 
        {
            pixels.clear(); 
        }
        
        pixels.show();

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}