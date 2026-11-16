#pragma once
#include "sensor_service.h"
#include <LiquidCrystal_I2C.h>

class DisplayService {
public:
  void begin();
  void render(const SensorSnapshot &snapshot, bool wifiConnected,
              bool mqttConnected, const char *ipAddress);
private:
  LiquidCrystal_I2C lcd_{0x27, 20, 4};
  uint8_t page_ = 0;
  uint32_t lastPageChangeMs_ = 0;
  uint32_t lastRenderMs_ = 0;
  void printLine(uint8_t row, const char *text);
  void renderAcceleration(const SensorSnapshot &snapshot);
  void renderGyroscope(const SensorSnapshot &snapshot);
  void renderLink(const SensorSnapshot &snapshot, bool wifiConnected,
                  bool mqttConnected, const char *ipAddress);
};
