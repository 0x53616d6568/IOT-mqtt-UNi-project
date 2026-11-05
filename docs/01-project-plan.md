# Project Plan

## Problem and objective

Build a small ESP32-based monitor that measures vibration on a fixed support, compares its behavior with a recorded normal reference, and flags sustained deviations. An HR202 threshold module provides a separate binary humidity-threshold signal. MQTT and Node-RED provide remote visibility and operator controls. The system is a demonstration of anomaly monitoring, not a certified safety device or a machine-fault diagnostic system.

## Goals

- Demonstrate a full sensor-to-dashboard IoT path.
- Implement defensible, documented signal features locally on the ESP32.
- Handle sensor validity, reference validity, event lifecycle, loss of connectivity, and recovery explicitly.
- Evaluate a conventional threshold baseline and optionally an XGBoost model on representative project-specific recordings.
- Deliver reproducible evidence rather than unsupported accuracy claims.

## Deliverables

1. Wired prototype with MPU6050/GY-521, HR202, ESP32, and red/green indicator LEDs.
2. Commented ESP32 source, build notes, pin map, and calibration/setup procedure.
3. Mosquitto/ MQTT topic and JSON contract; Node-RED flow export and dashboard.
4. Data collection protocol, feature definitions, test results, and optional model artifact/export.
5. Report (course brief asks 6–10 pages) and a 10–15 minute demonstration.

## Constraints and assumptions

- The actual module revision and board pinout must be confirmed before assigning wires or voltage levels.
- HR202 is treated only as a digital threshold output, not a quantitative humidity sensor.
- The red/green LEDs are simulated indicators; do not connect a real motor or machinery interlock.
- Sampling and feature extraction are local; MQTT is for slower telemetry and commands.
- Public datasets are useful for offline technique exploration but do not guarantee transfer to this sensor, mounting, and machine.
- Data collection should use a safe, stable fixture and controlled perturbations that cannot damage equipment or injure anyone.

## Suggested schedule

| Stage | Work | Exit evidence |
|---|---|---|
| 1. Scope/setup | Confirm hardware, define use case and safe fixture | Parts list, setup notes, repo structure |
| 2. Acquisition | Wire and verify MPU/HR202; log readings | Pin map, sample-rate/jitter note, raw capture |
| 3. Detection | Features, reference, thresholds, optional classifier | Python/device consistency, held-out runs |
| 4. Events | State machine, LEDs, acknowledgment and clear | State transition test log |
| 5. MQTT/UI | Broker, schema, controls, Node-RED | Flow export and end-to-end demonstration |
| 6. Validation | Faults, reconnects, invalid data, repeated trials | Test matrix and report evidence |

The hours and calendar duration depend on prior experience, hardware access, and whether the optional model is implemented. Treat this stage plan as a sequence, not a promise of completion time.

## Success criteria

- A normal reference can be created and invalidated safely.
- Stable vibration features can be computed repeatedly from fixed operating conditions.
- A controlled perturbation produces a confirmed event according to documented thresholds/model and confirmation rules.
- Ordinary handling and repeated runs are used to quantify false positives and missed detections.
- Invalid sensor data, missing reference, and stale telemetry are visible and cannot be presented as a normal measurement.
- MQTT state reflects the ESP32's actual applied state; UI commands do not imply success until confirmed.
- Results can be reproduced from the setup, source, data manifest, model/configuration, and test procedure.