#include "app_config.h"
#include "display_service.h"
#include "mqtt_service.h"
#include "sensor_service.h"
#include <Arduino.h>
#include <WiFi.h>

namespace {
SensorService sensorService;
DisplayService displayService;
MqttService mqttService;
SensorSnapshot latestSnapshot;
uint32_t lastSampleMs = 0;
uint32_t lastTelemetryMs = 0;
uint32_t lastWifiAttemptMs = 0;

void maintainWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  const uint32_t now = millis();
  if (lastWifiAttemptMs != 0 &&
      now - lastWifiAttemptMs < AppConfig::WIFI_RETRY_MS) return;

  lastWifiAttemptMs = now;
  Serial.printf("[WiFi] Connecting to %s ...\n", AppConfig::WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AppConfig::WIFI_SSID, AppConfig::WIFI_PASSWORD);
}
} // namespace

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== ESP32 equipment monitor: Wokwi acquisition test ===");

  displayService.begin();
  if (!sensorService.begin()) {
    Serial.println("[BOOT] MPU6050 unavailable; diagnostics and network remain active.");
  }

  WiFi.setAutoReconnect(true);
  maintainWiFi();
  mqttService.begin();
}

void loop() {
  const uint32_t now = millis();

  if (now - lastSampleMs >= AppConfig::SENSOR_SAMPLE_PERIOD_MS) {
    lastSampleMs = now;
    sensorService.sample(latestSnapshot);
  }

  maintainWiFi();
  mqttService.loop();

  if (now - lastTelemetryMs >= AppConfig::TELEMETRY_PERIOD_MS) {
    lastTelemetryMs = now;
    mqttService.publishTelemetry(latestSnapshot);
    Serial.printf("[SAMPLE] #%lu valid=%s HR202=%s\n",
                  static_cast<unsigned long>(latestSnapshot.sequence),
                  latestSnapshot.valid ? "yes" : "no",
                  latestSnapshot.hr202ThresholdActive ? "ACTIVE" : "inactive");
  }

  const String ip = WiFi.status() == WL_CONNECTED
                        ? WiFi.localIP().toString() : String("-");
  displayService.render(latestSnapshot, WiFi.status() == WL_CONNECTED,
                        mqttService.connected(), ip.c_str());
  delay(1);
}
