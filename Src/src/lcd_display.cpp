#include "lcd_display.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcdDisplay(void *pvParameters) 
{
    lcd.init();
    lcd.backlight();
    
    while (1) 
    {
        lcd.clear();
        
        // Temp & Humi
        lcd.setCursor(0, 0);
        lcd.printf("T:%.1fC H:%.1f%%", glob_temperature, glob_humidity);
        
        // Light
        lcd.setCursor(0, 1);
        lcd.printf("Light: %d%%   ", glob_light);
        // lcd.printf("Light: %d", glob_light);
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}