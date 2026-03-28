#include "luminous_intensity.h"

void luminousIntensity(void *pvParameters)
{
    pinMode(LIGHT_SENSOR, INPUT);
    while (1) 
    {
        int raw_light = analogRead(LIGHT_SENSOR);
        glob_light = map(raw_light, 0, 4095, 0, 100); // Map to '%'
        // glob_light = analogRead(LIGHT_SENSOR);
        
        Serial.print("Luminous Intensity: ");
        Serial.print(glob_light);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}