#include "luminous_intensity.h"

void luminousIntensity(void *pvParameters)
{
    pinMode(LIGHT_SENSOR, INPUT);
    while (1) 
    {
        glob_light = analogRead(LIGHT_SENSOR);
        
        Serial.print("Luminous Intensity: ");
        Serial.print(glob_light);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}