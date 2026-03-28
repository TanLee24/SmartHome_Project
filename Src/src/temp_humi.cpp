#include "temp_humi.h"

DHT20 dht20;

void temp_humi(void *pvParameters)
{
    dht20.begin();

    while (1) 
    {
        dht20.read();
        
        float temperature = dht20.getTemperature();
        float humidity = dht20.getHumidity();

        // Check if failed to read
        if (isnan(temperature) || isnan(humidity)) 
        {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity = -1;
        }
         else 
         {
            // Update global vars
            glob_temperature = temperature;
            glob_humidity = humidity;

            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.print("%  Temperature: ");
            Serial.print(temperature);
            Serial.println("°C");
        }
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}