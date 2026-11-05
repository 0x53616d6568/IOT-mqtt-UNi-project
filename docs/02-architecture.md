# Architecture

## Components

| Component | Responsibility |
|---|---|
| MPU6050 / GY-521 | Acceleration and gyroscope measurements. Project brief centers vibration analysis on acceleration. |
| HR202 module | Binary threshold input; polarity and output voltage must be validated on the exact module. |
| ESP32 | Sampling, input validation, windowing, feature computation, reference/model decision, event state machine, local LED outputs, MQTT client. |
| Green LED | Simulated normal/run-permission indicator only. |
| Red LED | Simulated anomaly/alert indicator only. |
| Mosquitto on PC | MQTT broker reachable by the ESP32 using the PC's LAN IP. |
| Node-RED on PC | MQTT flows, history/event journal, controls, and dashboard. |

## Data flow

1. ESP32 samples acceleration at a measured and documented cadence and reads HR202 state.
2. Samples are timestamped, validated, and grouped into fixed windows. Missing/invalid samples mark the window invalid rather than silently substituting zeros.
3. Firmware removes each acceleration axis's window mean and calculates RMS and maximum absolute amplitude. A combined magnitude can be added only if its definition is recorded and consistently used in training and firmware.
4. The detector compares features with a normal reference and uses either a simple threshold baseline or an optional exported XGBoost classifier. A persistence rule prevents a single transient from immediately becoming a confirmed alarm.
5. A local state machine updates event state and LEDs regardless of connectivity.
6. ESP32 publishes telemetry, state, events, and availability over MQTT. Commands are validated locally and actual applied state is returned.
7. Node-RED displays sensor/features, state, alerts, history, connection freshness, reference/model status, and controls.

## Layer boundaries

- **Acquisition:** raw sensor reads and timing/validity.
- **Feature extraction:** pure deterministic feature code; test against Python reference.
- **Decision:** baseline/model output + confidence/threshold + persistence.
- **Event management:** state transitions, event IDs, acknowledgement, clear.
- **Transport:** MQTT serialization, reconnect, availability, command validation.
- **Presentation:** dashboard; it must show confirmed device state, not infer that a command succeeded.

## Connectivity principle

An MQTT outage must not stop local sampling, local anomaly decisions, or LED behavior. Publish after reconnection and indicate stale/offline status on the dashboard. A bounded buffer is optional; if used, define overflow behavior and do not replay an unbounded queue.

## Configuration items to record

- Board and module revisions; I²C address and selected bus pins.
- Sample target and measured interval/jitter; window length and stride.
- Axis/range configuration, units, feature order, and scaling.
- Reference procedure, anomaly threshold or model version, confirmation and clear times.
- MQTT broker address, client ID, keepalive, topics, and security configuration (never commit passwords).
- Firmware version and Node-RED flow version.