#include "lcd_display.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcdDisplay(void *pvParameters) 
{
    lcd.init();
    lcd.backlight();
    
    while (1) 
    {
        lcd.clear();
        
        // Dòng 1: Nhiệt độ & Độ ẩm
        lcd.setCursor(0, 0);
        // Dùng hàm printf cho gọn mã code
        lcd.printf("T:%.1fC H:%.1f%%", glob_temperature, glob_humidity);
        
        // Dòng 2: Ánh sáng
        lcd.setCursor(0, 1);
        lcd.printf("Light: %d", glob_light);
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}