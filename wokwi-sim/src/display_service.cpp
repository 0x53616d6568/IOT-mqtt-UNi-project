#include "display_service.h"
#include "app_config.h"
#include <Arduino.h>
#include <cstdio>

void DisplayService::begin() {
  lcd_.init();
  lcd_.backlight();
  for (uint8_t row = 0; row < AppConfig::LCD_ROWS; ++row) {
    printLine(row, "Starting sensor test");
  }
}

void DisplayService::printLine(uint8_t row, const char *text) {
  lcd_.setCursor(0, row);
  char line[AppConfig::LCD_COLUMNS + 1]{};
  snprintf(line, sizeof(line), "%-20.20s", text);
  lcd_.print(line);
}

void DisplayService::render(const SensorSnapshot &snapshot,
                            bool wifiConnected, bool mqttConnected,
                            const char *ipAddress) {
  const uint32_t now = millis();
  if (now - lastPageChangeMs_ >= AppConfig::LCD_PAGE_PERIOD_MS) {
    page_ = static_cast<uint8_t>((page_ + 1U) % 3U);
    lastPageChangeMs_ = now;
  }
  if (now - lastRenderMs_ < 250U) {
    return;
  }
  lastRenderMs_ = now;

  switch (page_) {
  case 0: renderAcceleration(snapshot); break;
  case 1: renderGyroscope(snapshot); break;
  default: renderLink(snapshot, wifiConnected, mqttConnected, ipAddress); break;
  }
}

void DisplayService::renderAcceleration(const SensorSnapshot &s) {
  char line[32];
  printLine(0, "ACCELERATION (m/s2)");
  snprintf(line, sizeof(line), "X:%7.2f Y:%7.2f", s.accelX, s.accelY);
  printLine(1, line);
  snprintf(line, sizeof(line), "Z:%7.2f  %s", s.accelZ,
           s.valid ? "VALID" : "INVALID");
  printLine(2, line);
  printLine(3, s.hr202ThresholdActive ? "HR202: THRESHOLD ON"
                                      : "HR202: threshold off");
}

void DisplayService::renderGyroscope(const SensorSnapshot &s) {
  char line[32];
  printLine(0, "GYRO (rad/s)");
  snprintf(line, sizeof(line), "X:%7.2f Y:%7.2f", s.gyroX, s.gyroY);
  printLine(1, line);
  snprintf(line, sizeof(line), "Z:%7.2f  %s", s.gyroZ,
           s.valid ? "VALID" : "INVALID");
  printLine(2, line);
  printLine(3, "MPU6050 / GY-521");
}

void DisplayService::renderLink(const SensorSnapshot &s, bool wifiConnected,
                                bool mqttConnected, const char *ipAddress) {
  char line[32];
  printLine(0, "COMMUNICATION STATUS");
  snprintf(line, sizeof(line), "WiFi: %s",
           wifiConnected ? "CONNECTED" : "OFFLINE");
  printLine(1, line);
  snprintf(line, sizeof(line), "MQTT: %s",
           mqttConnected ? "CONNECTED" : "OFFLINE");
  printLine(2, line);
  snprintf(line, sizeof(line), "IP:%s #%lu",
           wifiConnected ? ipAddress : "-",
           static_cast<unsigned long>(s.sequence));
  printLine(3, line);
}
