# ESP32 Smart Home System

A comprehensive Smart Home IoT project built with ESP32, FreeRTOS, and MQTT. This system monitors environmental conditions, detects motion, features an IR-controlled secure door lock, and integrates with a cloud dashboard (Core IoT) for remote monitoring and control.

## 🌟 Features

* **Real-time Environment Monitoring:** Measures temperature and humidity using a DHT20 sensor, and luminous intensity using an analog light sensor.
* **Motion Detection:** Utilizes a PIR sensor to detect human presence and triggers a local indicator LED.
* **Secure Door Access Control:** Uses an IR remote to input passwords and control a servo-based door lock. It supports state-machine-based operations (Idle, Open Door, Change Password).
* **Local Display:** Shows real-time temperature, humidity, and light intensity on a 16x2 I2C LCD.
* **IoT Cloud Integration:** Connects to the Core IoT platform via MQTT to send telemetry data and receive RPC (Remote Procedure Call) commands.
* **Remote Actuator Control:** Controls an RGB NeoPixel LED and adjusts a DC Fan's speed via PWM based on cloud commands.
* **Multitasking:** Implemented using FreeRTOS to ensure non-blocking, concurrent execution of tasks (reading sensors, updating LCD, handling MQTT, and IR decoding).

## 🛠 Hardware Requirements

* **Microcontroller:** ESP32 Development Board
* **Sensors:**
    * DHT20 (Temperature & Humidity)
    * Analog Light Sensor (Photoresistor module)
    * PIR Motion Sensor (HC-SR501 or similar)
    * IR Receiver (VS1838B or similar)
* **Actuators & Outputs:**
    * Servo Motor (SG90/MG996R for door mechanism)
    * 16x2 LCD Display with I2C Backpack
    * NeoPixel RGB LED (WS2812B)
    * DC Fan (requires suitable motor driver or transistor)
    * Standard LED (for motion indication)
* **Other:** IR Remote Control, jumper wires, breadboard.

## 📌 Pin Configuration

| Component | ESP32 GPIO | Notes |
| :--- | :--- | :--- |
| **I2C SDA (LCD & DHT20)** | `GPIO 21` | I2C Data line |
| **I2C SCL (LCD & DHT20)** | `GPIO 22` | I2C Clock line |
| **Light Sensor** | `GPIO 33` | Analog Input |
| **PIR Sensor** | `GPIO 2` | Digital Input |
| **IR Receiver** | `GPIO 27` | Digital Input |
| **Servo Motor** | `GPIO 5` | PWM Output (50Hz) |
| **DC Fan** | `GPIO 26` | PWM Output (Channel 0) |
| **NeoPixel RGB** | `GPIO 32` | Digital Output |
| **Single LED (PIR Indicator)**| `GPIO 19` | Digital Output |

*(Note: Pin definitions can be modified in `include/global.h`)*

## 💻 Software & Libraries

This project is developed using **PlatformIO**. The following libraries are required and will be automatically downloaded by PlatformIO based on the `platformio.ini` file:

* [PubSubClient](https://github.com/knolleary/pubsubclient) (MQTT protocol)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson) (JSON serialization/deserialization)
* [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) (RGB LED control)
* [DHT20](https://github.com/RobTillaart/DHT20) (Temperature & Humidity)
* [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) (LCD display)
* [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) (Hardware PWM servo control)
* [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) (IR receiving and decoding)

## 🚀 System Architecture (FreeRTOS Tasks)

The system is divided into independent FreeRTOS tasks to optimize performance:
1.  **Task Temperature Humidity:** Reads DHT20 sensor every 5 seconds.
2.  **Task Luminous Intensity:** Reads analog light sensor and maps it to a percentage every 5 seconds.
3.  **Task LCD Display:** Refreshes the 16x2 LCD with the latest global variables every 5 seconds.
4.  **Task Detect Human:** Polls the PIR sensor every 200ms and toggles the indicator LED.
5.  **Task Door Control:** Listens for IR signals, decodes keypad inputs, manages the password state machine, and controls the servo motor.
6.  **Task MQTT Connect:** Maintains WiFi/MQTT connection, publishes telemetry every 5 seconds, and processes incoming RPC commands (Fan speed, LED color, Remote door opening).

## ⚙️ Getting Started

### 1. Clone the Repository
```bash
git clone <your-repository-url>
cd SmartHome_Project