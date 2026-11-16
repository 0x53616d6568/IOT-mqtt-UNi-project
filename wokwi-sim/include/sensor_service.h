#pragma once
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <array>
#include <cstddef>
#include <cstdint>

struct SensorSnapshot {
  uint32_t sequence = 0;
  uint32_t uptimeMs = 0;
  bool valid = false;
  bool hr202ThresholdActive = false;
  float accelX = 0.0F, accelY = 0.0F, accelZ = 0.0F; // m/s^2
  float gyroX = 0.0F, gyroY = 0.0F, gyroZ = 0.0F;    // rad/s
  bool featuresReady = false;
  float rmsX = 0.0F, rmsY = 0.0F, rmsZ = 0.0F;
  float maxAbsX = 0.0F, maxAbsY = 0.0F, maxAbsZ = 0.0F;
};

class SensorService {
public:
  bool begin();
  bool sample(SensorSnapshot &snapshot);
private:
  static constexpr size_t WINDOW_SIZE = 50;
  Adafruit_MPU6050 mpu_;
  bool initialized_ = false;
  size_t writeIndex_ = 0;
  size_t samplesInWindow_ = 0;
  std::array<std::array<float, WINDOW_SIZE>, 3> accelWindow_{};
  void calculateFeatures(SensorSnapshot &snapshot) const;
};
