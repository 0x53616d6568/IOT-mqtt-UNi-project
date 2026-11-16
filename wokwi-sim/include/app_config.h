#pragma once
#include <Arduino.h>

// ESP32 DevKit V1 / Wokwi pin map. Verify the exact physical board before wiring.
namespace AppConfig {
constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;
constexpr uint8_t HR202_DIGITAL_PIN = 27; // Button emulates active-low digital OUT.
constexpr uint8_t GREEN_LED_PIN = 25;
constexpr uint8_t RED_LED_PIN = 26;
constexpr uint8_t MPU6050_I2C_ADDRESS = 0x68;
constexpr uint8_t LCD_I2C_ADDRESS = 0x27;
constexpr uint8_t LCD_COLUMNS = 20;
constexpr uint8_t LCD_ROWS = 4;
constexpr uint32_t SENSOR_SAMPLE_PERIOD_MS = 20; // 50 Hz target.
constexpr uint32_t TELEMETRY_PERIOD_MS = 1000;
constexpr uint32_t LCD_PAGE_PERIOD_MS = 2500;
constexpr size_t FEATURE_WINDOW_SAMPLES = 50; // One second at 50 Hz.
constexpr char DEVICE_ID[] = "esp32-equip-wokwi";
constexpr char MQTT_TOPIC_PREFIX[] = "iot/groupe6/equipement";
constexpr char MQTT_BROKER_HOST[] = "broker.hivemq.com";
constexpr uint16_t MQTT_BROKER_PORT = 1883;
constexpr uint16_t MQTT_BUFFER_SIZE = 768;
constexpr uint32_t WIFI_RETRY_MS = 10000;
constexpr uint32_t MQTT_RETRY_MS = 5000;
// Wokwi virtual AP. Replace only for local hardware; never commit real credentials.
constexpr char WIFI_SSID[] = "Wokwi-GUEST";
constexpr char WIFI_PASSWORD[] = "";
}
