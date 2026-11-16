#pragma once
#include "sensor_service.h"
#include <PubSubClient.h>
#include <WiFi.h>

class MqttService {
public:
  void begin();
  void loop();
  bool connected() const;
  void publishTelemetry(const SensorSnapshot &snapshot);
private:
  WiFiClient networkClient_;
  PubSubClient mqtt_{networkClient_};
  uint32_t lastReconnectAttemptMs_ = 0;
  char telemetryTopic_[96]{};
  char statusTopic_[96]{};
  char clientId_[48]{};
  bool connect();
};
