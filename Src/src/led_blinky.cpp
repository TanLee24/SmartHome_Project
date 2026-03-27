#include "led_blinky.h"

void ledBlinky(void *pvParameters)
{
    pinMode(LED_GPIO, OUTPUT);

    while (1)
    {
        digitalWrite(LED_GPIO, HIGH); // ON
        vTaskDelay(1000);
        digitalWrite(LED_GPIO, LOW); // OFF
        vTaskDelay(1000);
    }
}