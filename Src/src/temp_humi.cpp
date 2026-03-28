#include "temp_humi.h"

DHT20 dht20;

void temp_humi(void *pvParameters)
{
    dht20.begin();

    while (1) 
    {
        // Yêu cầu cảm biến đọc dữ liệu
        dht20.read();
        
        // Lấy giá trị nhiệt độ (Celsius) và độ ẩm (%)
        float temperature = dht20.getTemperature();
        float humidity = dht20.getHumidity();

        // Kiểm tra xem quá trình đọc có bị lỗi (trả về NaN) hay không
        if (isnan(temperature) || isnan(humidity)) 
        {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity = -1;
        }
         else 
         {
            // Cập nhật biến toàn cục
            glob_temperature = temperature;
            glob_humidity = humidity;

            // In kết quả ra Serial Monitor
            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.print("%  Temperature: ");
            Serial.print(temperature);
            Serial.println("°C");
        }
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}