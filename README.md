# IoT MQTT University Project — ESP32 Equipment Monitor

> **Planning status:** repository plan and documentation scaffold. Hardware pin assignments, sampling rate, window length, thresholds, event timings, credentials, and final model must be validated on the actual hardware before implementation.

This repository plans **Project 6: Industrial Equipment Monitoring**. The prototype uses an ESP32, MPU6050/GY-521 inertial sensor, HR202 threshold module, indicator LEDs, MQTT/Mosquitto, and a Node-RED dashboard. It detects repeatable changes in vibration; it does **not** diagnose a specific mechanical failure or control real machinery.

## System explanation

The MPU6050 measures acceleration on the monitored fixture. The ESP32 samples the sensor locally, validates the readings, divides them into fixed windows, removes the per-axis mean, and calculates vibration features including RMS and maximum amplitude. It compares those features with a recorded normal reference using a transparent threshold detector first. An optional small XGBoost model may later classify the same project-specific features; if used, it must be tested against a held-out set of independent sessions and compared with the threshold baseline.

The HR202 is read as a **binary threshold state**, not as a calibrated humidity percentage. The ESP32 combines sensor validity, reference availability, vibration decisions, and HR202 state in a local event/state machine. Local sampling, decisions, and simulated LED indications continue even when Wi-Fi or MQTT is unavailable. The green LED is only a simulated normal/run-permission indicator; the red LED is only a simulated alarm indicator.

The ESP32 publishes measurements, confirmed state, event transitions, and online/offline status through MQTT. Mosquitto runs on the PC, and the ESP32 connects to the PC using its LAN IP address. Node-RED subscribes to the MQTT topics and presents vibration features, sensor health, current state, stale-data status, alerts, history, and controls. Dashboard commands are requests: the UI must wait for `/state` to show what the ESP32 actually applied.

```text
MPU6050 + HR202
      │ I²C + digital threshold input
      ▼
ESP32: acquisition → validation → windows/features → baseline / optional XGBoost
      │                         │
      │ local event/state       └── green/red LEDs (simulation only)
      │
      └── Wi-Fi / MQTT → Mosquitto on PC → Node-RED dashboard
```

**Safety and claims:** this is a lab demonstrator, not a safety interlock or predictive maintenance product. A detected anomaly is evidence of a change in measured vibration, not proof of a fault. Public bearing datasets can support offline exploration, but project-specific training and evaluation data should come from the actual MPU6050, mounting, and test setup.

## Goals

- Acquire repeatable, timestamped acceleration measurements at a measured and documented sample rate.
- Compute per-window RMS and maximum absolute amplitude after removing each axis's mean.
- Record a reproducible normal reference and detect sustained changes with documented confirmation/clear rules.
- Read HR202 as a binary threshold input, keeping it distinct from vibration analysis.
- Keep acquisition and decisions local; use MQTT for remote telemetry, state, alerts, and commands.
- Make Node-RED display confirmed device state, connection freshness, active alerts, and history.
- Validate results with repeatable trials, report false alarms and limitations, and make the demo reproducible.

## Repository map

```text
.
├── README.md                         # Main project page: system explanation + TODO checklist
├── TODO.md                           # Standalone milestone checklist
└── docs/
    ├── 01-project-plan.md
    ├── 02-architecture.md
    ├── 03-wiring.md
    ├── 04-anomaly-detection.md
    ├── 05-event-handling.md
    ├── 06-mqtt-contract.md
    ├── 07-dashboard.md
    └── 08-test-plan.md
```

## TODO and milestones

Use `[ ]` for not started and `[x]` when complete; add a short evidence note or commit reference when checking an item. **P0** blocks a working prototype; **P1** is expected for a strong submission; **P2** is optional polish.

### M0 — Scope and setup
- [ ] **P0** Confirm the exact ESP32 board, MPU6050/GY-521, HR202, LEDs, resistors, jumper wires, and PC.
- [ ] **P0** Record parts, module labels, and pin names; agree the project detects anomalies, not machine faults.
- [ ] **P1** Choose and document a safe, repeatable vibration fixture.
- [ ] **P1** Create implementation folders for firmware, Node-RED, data, models, and reports when work begins.

### M1 — Hardware and acquisition
- [ ] **P0** Verify module supply voltage, logic levels, common ground, and actual board pinout before wiring.
- [ ] **P0** Assign and record GPIO/I²C pins after checking the exact hardware; verify MPU identity and bus operation.
- [ ] **P0** Verify HR202 output polarity and ESP32-safe voltage; test each LED through its own series resistor.
- [ ] **P0** Acquire timestamped acceleration and flag missing, repeated, or malformed readings.
- [ ] **P1** Measure sample interval/jitter, choose a documented target rate and window length, and save a raw-data capture.

### M2 — Features and baseline detector
- [ ] **P0** Remove each axis's mean per analysis window; calculate RMS and maximum absolute amplitude.
- [ ] **P0** Check firmware features against a Python reference using saved test vectors.
- [ ] **P0** Handle invalid sensor readings and missing/invalid reference safely; do not treat them as normal.
- [ ] **P1** Collect multiple independent normal runs and controlled, safely produced perturbation runs with labels and metadata.
- [ ] **P1** Implement an interpretable threshold plus persistence baseline; tune using held-out sessions.
- [ ] **P2** Explore public CWRU/NASA data for feature experiments only; document its mismatch with this sensor/setup.

### M3 — Optional XGBoost edge model
- [ ] **P1** Define fixed features, feature order, units, labels, and independent session-level train/validation/test splits.
- [ ] **P1** Train a small XGBoost classifier on project-collected data and keep the threshold baseline for comparison.
- [ ] **P1** Evaluate held-out precision/recall, confusion matrix, and false alerts per test duration.
- [ ] **P1** Try C export using `m2cgen`; verify generated C against Python on identical vectors within a stated tolerance.
- [ ] **P1** Measure ESP32 code size, inference time, and RAM; reduce tree count/depth if needed.
- [ ] **P2** Version the model, preprocessing, class order, feature schema, dependencies, and data manifest.

### M4 — Local state machine and event handling
- [ ] **P0** Implement startup-unvalidated, no-reference, normal, vigilance, anomaly, and invalid-data states.
- [ ] **P0** Keep local decisions independent of MQTT; ensure invalid conditions cannot assert the green indicator.
- [ ] **P0** Map LEDs to documented simulated states only.
- [ ] **P1** Add event IDs, timestamps/uptime, active/cleared/acknowledged states, and duplicate suppression.
- [ ] **P1** Ensure acknowledgement does not hide an event whose condition remains active.
- [ ] **P1** Test restart, sensor loss, Wi-Fi loss, and broker loss behavior.

### M5 — MQTT and Node-RED
- [ ] **P0** Configure Mosquitto on the PC and verify the ESP32 connects using the PC's LAN IP.
- [ ] **P0** Implement topic prefix `iot/groupe6/equipement` and the JSON schema in `docs/06-mqtt-contract.md`.
- [ ] **P0** Implement Last Will/availability, documented retained messages, command validation, and confirmed applied state.
- [ ] **P1** Add reconnect behavior and stale-data indication.
- [ ] **P1** Build dashboard plots, status cards, controls, and event journal; export the Node-RED flow.
- [ ] **P1** Keep broker passwords and other credentials out of Git.

### M6 — Verification and delivery
- [ ] **P0** Run the required tests in `docs/08-test-plan.md` and record expected/observed results with evidence.
- [ ] **P1** Evaluate on held-out sessions; test normal operation, perturbations, invalid input, reconnect, and reboot.
- [ ] **P1** Document calibration, features, thresholds, results, false positives, limitations, and reproduction steps.
- [ ] **P1** Prepare a 10–15 minute demo: normal → safe perturbation → alert → acknowledge → return to normal → broker interruption/recovery.
- [ ] **P2** Tag/archive the final firmware, Node-RED flow, model, sanitized data manifest, and report.

| Milestone | Exit condition |
|---|---|
| M1 Hardware | Stable timestamped readings; verified wiring and logic levels documented. |
| M2 Detection | Repeatable features and baseline detector; invalid input handled safely. |
| M3 Edge AI (optional) | Held-out evaluation supports the model; generated C agrees with Python. |
| M4 Events | Confirm/acknowledge/clear behavior verified locally. |
| M5 IoT integration | MQTT, commands, state confirmation, availability, and dashboard work end-to-end. |
| M6 Delivery | Test evidence, report, source/flows, and demo are reproducible. |

## Detailed documentation

- [Project plan and deliverables](docs/01-project-plan.md)
- [System architecture and data flow](docs/02-architecture.md)
- [Schematic/wiring plan](docs/03-wiring.md) — pin assignments deliberately remain TBD until the board/modules are verified.
- [Anomaly detection and optional XGBoost/m2cgen](docs/04-anomaly-detection.md)
- [Event lifecycle and local state machine](docs/05-event-handling.md)
- [MQTT topics and JSON payloads](docs/06-mqtt-contract.md)
- [Node-RED dashboard requirements](docs/07-dashboard.md)
- [Test plan and demo sequence](docs/08-test-plan.md)

## Definition of done

The prototype has verified wiring and documented sampling; firmware handles valid/invalid data and runs locally without MQTT; features match the training pipeline; events and commands have confirmed state semantics; the dashboard distinguishes normal, active, stale, invalid, and offline conditions; and test evidence supports every reported result. No accuracy, fault-diagnosis, or safety claim is made without appropriate independent verification.