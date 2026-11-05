# TODO and Milestones

Use `[ ]` for not started, `[x]` for completed, and add a short evidence note or commit reference when checking an item. Priorities: **P0** blocks the prototype; **P1** required for a strong submission; **P2** polish/optional.

## M0 — Scope and setup
- [ ] **P0** Confirm supplied ESP32 board, MPU6050/GY-521 module, HR202 module, LEDs, resistors, jumper wires, and PC.
- [ ] **P0** Create a parts list and photograph/record module labels and pin names.
- [ ] **P0** Agree that the output is anomaly detection, not machine fault diagnosis.
- [ ] **P1** Choose a small, safe vibration test fixture and document how it is operated.
- [ ] **P1** Create firmware, Node-RED, data, and report folders when implementation begins.

## M1 — Hardware and acquisition
- [ ] **P0** Read module documentation and verify supply voltage and logic levels.
- [ ] **P0** Decide and record ESP32 GPIO/I²C pins only after checking the exact board and modules.
- [ ] **P0** Verify common ground and test sensor bus scan / MPU identity.
- [ ] **P0** Verify HR202 digital threshold polarity and ESP32-safe output level before connection.
- [ ] **P0** Test each LED through its own series resistor.
- [ ] **P0** Acquire timestamped acceleration on all axes; detect missing, repeated, or malformed readings.
- [ ] **P1** Measure actual sample interval and jitter; choose a documented target rate/window length.
- [ ] **P1** Add serial diagnostics and save a short raw-data capture.

## M2 — Features and baseline detector
- [ ] **P0** Remove per-axis mean per analysis window.
- [ ] **P0** Compute RMS and maximum absolute amplitude per axis and/or a documented combined magnitude.
- [ ] **P0** Verify feature calculations with saved test vectors / a Python reference implementation.
- [ ] **P0** Add data-validity checks and a safe state for invalid sensor/reference data.
- [ ] **P1** Capture normal-operation sessions across multiple runs and ordinary handling conditions.
- [ ] **P1** Capture controlled, safe perturbation sessions with labels and run metadata.
- [ ] **P1** Implement a simple threshold + persistence detector as an interpretable baseline.
- [ ] **P1** Tune reference, thresholds, and confirmation/clear timing using held-out sessions.
- [ ] **P2** Explore public CWRU/NASA data for offline feature experiments only; document domain mismatch.

## M3 — Optional XGBoost edge model
- [ ] **P1** Define exactly which window features and labels the classifier uses.
- [ ] **P1** Split data by independent session/run before creating windows; do not randomly distribute neighboring windows across train and test.
- [ ] **P1** Train a small XGBoost classifier on the project's own labeled data; retain a threshold baseline.
- [ ] **P1** Evaluate recall, precision, false alarms per test duration, and confusion matrix on held-out sessions.
- [ ] **P1** Export to C with `m2cgen` if the supported estimator/version combination works.
- [ ] **P1** Compare Python and generated-C outputs on the same saved vectors within a stated tolerance.
- [ ] **P1** Measure firmware code size, inference time, and RAM use; reduce tree count/depth if needed.
- [ ] **P2** Version model, feature order, units, preprocessing, class mapping, and training data manifest together.

## M4 — Local state machine and events
- [ ] **P0** Implement startup-unvalidated, normal, vigilance, anomaly, and invalid-data behavior.
- [ ] **P0** Make local decisions independent of broker availability.
- [ ] **P0** Map green LED to permitted/normal and red LED to confirmed anomaly/alert as specified in the docs.
- [ ] **P1** Implement event IDs, timestamps/uptime, active/cleared/acknowledged status, and duplicate suppression.
- [ ] **P1** Ensure acknowledgement silences/resets only the annunciation if allowed; it must not hide a still-active condition.
- [ ] **P1** Test restart and sensor/broker loss behavior.

## M5 — MQTT and Node-RED
- [ ] **P0** Install/configure Mosquitto on the PC; verify ESP32 can connect using PC LAN IP.
- [ ] **P0** Use topic prefix `iot/groupe6/equipement` and JSON payloads from `docs/06-mqtt-contract.md`.
- [ ] **P0** Implement Last Will and availability; set retained state/availability only where documented.
- [ ] **P0** Validate incoming commands and publish resulting applied state; reject invalid commands.
- [ ] **P1** Add reconnect behavior and stale-data indication.
- [ ] **P1** Build Node-RED dashboard cards/plots/event journal from `docs/07-dashboard.md`.
- [ ] **P1** Export Node-RED flows and record required nodes/configuration without secrets.

## M6 — Verification and delivery
- [ ] **P0** Execute all P0 tests in `docs/08-test-plan.md`; save observed values and evidence.
- [ ] **P1** Execute session-held-out anomaly evaluation and compare with threshold baseline.
- [ ] **P1** Test Wi-Fi/broker interruption, stale telemetry, reconnection, reboot, and invalid input.
- [ ] **P1** Record results, limitations, thresholds, feature definitions, and reproducibility steps.
- [ ] **P1** Prepare a 10–15 minute demo: normal → repeatable perturbation → alert → acknowledge → return to normal → broker loss/recovery.
- [ ] **P2** Tag a final version and archive firmware, flow export, data manifest, model, and report.

## Milestone acceptance

| Milestone | Exit condition |
|---|---|
| M1 Hardware | Stable, timestamped sensor readings; wiring and levels documented. |
| M2 Detection | Repeatable baseline features and detector; invalid data handled safely. |
| M3 Edge AI (optional) | Model beats or complements baseline on held-out runs and matches generated C behavior. |
| M4 Events | Confirm/clear/acknowledge behavior tested locally, including persistent conditions. |
| M5 IoT integration | Telemetry, commands, state confirmation, availability, and dashboard work end-to-end. |
| M6 Delivery | Test evidence, report, code/flows, and demo steps are reproducible. |