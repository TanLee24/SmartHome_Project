#include "mqtt_connect.h"

const char* ssid = "TannLee";
const char* password = "12345678";
const char* mqtt_server = "app.coreiot.io";
const char* mqtt_token = "h8pmt902y143ltzgdd3h";
const int mqtt_port = 1883;

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);

// Biến lưu trạng thái thiết bị nhận từ server
bool led_status = false;
uint8_t led_r = 255, led_g = 255, led_b = 255;
bool fan_status = false;
int fan_speed = 100; // Tốc độ quạt mặc định (0 - 100%)

// Hàm cập nhật trạng thái ra chân phần cứng
void updateHardware() {
    // 1. Cập nhật LED RGB
    if (led_status) {
        for(int i = 0; i < NUMPIXELS; i++) {
            pixels.setPixelColor(i, pixels.Color(led_r, led_g, led_b));
        }
    } else {
        pixels.clear(); // Tắt LED
    }
    pixels.show();

    // 2. Cập nhật Quạt
    if (fan_status) {
        // Quy đổi % (0-100) sang dải băm xung PWM (0-255)
        int pwm_val = map(fan_speed, 0, 100, 0, 255);
        ledcWrite(FAN_PWM_CHANNEL, pwm_val);
    } else {
        ledcWrite(FAN_PWM_CHANNEL, 0); // Tắt quạt
    }
}

// Hàm Callback: Xử lý khi có lệnh từ Server gửi xuống
void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.printf("Nhận lệnh RPC từ topic: %s\n", topic);
    Serial.printf("Nội dung: %s\n", message);

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.println("Lỗi phân tích JSON!");
        return;
    }

    String method = doc["method"].as<String>();

    // Phân tích các hàm điều khiển
    if (method == "setLedStatus") {
        led_status = doc["params"].as<bool>();
    } 
    else if (method == "setRed") {
        led_r = doc["params"].as<uint8_t>(); // Nhận giá trị 0-255 cho màu Đỏ
    } 
    else if (method == "setGreen") {
        led_g = doc["params"].as<uint8_t>(); // Nhận giá trị 0-255 cho màu Xanh lá
    } 
    else if (method == "setBlue") {
        led_b = doc["params"].as<uint8_t>(); // Nhận giá trị 0-255 cho màu Xanh dương
    }
    else if (method == "setFanStatus") {
        fan_status = doc["params"].as<bool>();
    } 
    else if (method == "setFanSpeed") {
        fan_speed = doc["params"].as<int>();
    }
    else if (method == "openDoor") {
        Serial.println("FaceAI: Open the door");
        glob_open_door_request = true; // Phất cờ yêu cầu mở cửa
    }

    // Thực thi lệnh ra phần cứng
    updateHardware();

    // Phản hồi lại server để xác nhận đã xử lý xong (Tránh lỗi timeout trên web)
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), "{\"status\":\"success\"}");
}

// Hàm kết nối WiFi và MQTT
void reconnect() {
    // Kết nối WiFi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Đang kết nối WiFi");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(pdMS_TO_TICKS(500));
            Serial.print(".");
        }
        Serial.println("\nWiFi đã kết nối!");
    }

    // Kết nối MQTT
    while (!client.connected()) {
        Serial.print("Đang kết nối Core IoT Server... ");
        // Tham số thứ 2 là Access Token (username), password để trống
        if (client.connect("ESP32_SmartHome", mqtt_token, "")) {
            Serial.println("Thành công!");
            // Đăng ký nhận lệnh RPC
            client.subscribe("v1/devices/me/rpc/request/+");
        } else {
            Serial.print("Thất bại, mã lỗi: ");
            Serial.print(client.state());
            Serial.println(" Thử lại sau 3 giây...");
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
    }
}

void mqttConnect(void *pvParameters) 
{
    // Cài đặt chân phần cứng
    pixels.begin();
    pixels.clear();
    pixels.show();

    // Cài đặt kênh PWM cho Quạt (ESP32 API)
    ledcSetup(FAN_PWM_CHANNEL, FAN_PWM_FREQ, FAN_PWM_RES);
    ledcAttachPin(FAN_PIN, FAN_PWM_CHANNEL);
    ledcWrite(FAN_PWM_CHANNEL, 0);

    // Cài đặt MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    unsigned long lastMsg = 0;

    while (1) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        unsigned long now = millis();
        // Cứ mỗi 5 giây, đóng gói dữ liệu và gửi lên server
        if (now - lastMsg > 5000) {
            lastMsg = now;

            // Đóng gói JSON dữ liệu viễn trắc (Telemetry)
            StaticJsonDocument<256> doc;
            doc["temperature"] = glob_temperature;
            doc["humidity"] = glob_humidity;
            doc["light"] = glob_light;
            doc["human_detected"] = glob_human_detected;

            char buffer[256];
            serializeJson(doc, buffer);

            // Gửi dữ liệu lên Core IoT
            client.publish("v1/devices/me/telemetry", buffer);
            Serial.print("Đã gửi Telemetry: ");
            Serial.println(buffer);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Tránh lỗi Watchdog Timer
    }
}