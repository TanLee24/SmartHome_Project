#include "human_detect.h"


void humanDetect(void *pvParameters) 
{
    pinMode(PIR_PIN, INPUT);
    pinMode(SINGLE_LED_PIN, OUTPUT);
    
    digitalWrite(SINGLE_LED_PIN, LOW);

    while (1) 
    {
        // 1 = Detected, vice versa
        bool is_detected = digitalRead(PIR_PIN);
        
        glob_human_detected = is_detected;

        if (is_detected) 
        {
            // Human detected (Moving) -> LED On
            digitalWrite(SINGLE_LED_PIN, HIGH);
            Serial.println("PIR: Human Detected!");
        } 
        else 
        {
            // No moving -> LED Off
            digitalWrite(SINGLE_LED_PIN, LOW);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}