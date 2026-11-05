# Test and Validation Plan

Record for every test: test ID, firmware/model/config version, setup, procedure, expected result, observed result, count/duration, pass/fail, and evidence path. Do not fill results until tests are actually performed.

## Hardware and acquisition

| ID | Test | Expected result |
|---|---|---|
| HW-01 | Power-on and I²C scan | MPU6050 identified at documented address; no unstable bus. |
| HW-02 | Stationary acquisition | Samples arrive at a measurable cadence; timestamp gaps/jitter are recorded. |
| HW-03 | Axis/orientation check | Moving each axis changes the expected channel/sign; mounting axes documented. |
| HW-04 | HR202 threshold transition | Both logic states and polarity are verified; ESP32 input stays within safe logic levels. |
| HW-05 | LED outputs | Each LED operates through its series resistor; startup outputs remain off until state is valid. |
| HW-06 | Sensor unavailable | Missing/invalid readings are flagged and never converted into normal/zero values. |

## Features and detector

| ID | Test | Expected result |
|---|---|---|
| ML-01 | Python vs firmware feature vectors | RMS and maximum absolute amplitude match within stated numeric tolerance. |
| ML-02 | Repeat normal run | Feature distribution is reasonably repeatable; run conditions and spread are documented. |
| ML-03 | Ordinary handling | Normal handling does not create excessive alerts; false alarms are counted. |
| ML-04 | Controlled perturbation | Defined perturbation creates candidate/confirmed event according to threshold/persistence rules. |
| ML-05 | Brief transient | A short transient follows documented candidate/confirmation policy; no accidental duplicate event. |
| ML-06 | Return to normal | Event clears only after clear criteria/duration are met. |
| ML-07 | No/missing reference | Detector reports no-reference/unknown and does not claim normal. |
| ML-08 | Held-out evaluation | Runs reserved by session are evaluated; report confusion matrix and false alerts per duration. |
| ML-09 | Generated-C equivalence (optional) | Python and m2cgen C outputs match on frozen vectors within documented tolerance. |
| ML-10 | Resource check (optional) | ESP32 inference time, code size, and memory use are measured. |

## Event and command handling

| ID | Test | Expected result |
|---|---|---|
| EVT-01 | Confirm a candidate | Event created once after required persistence; unique ID and cause published. |
| EVT-02 | Acknowledge while condition persists | Acknowledged status changes, but condition stays active/visible until clear. |
| EVT-03 | Clear condition | Clear transition recorded after clear criteria; history retained. |
| EVT-04 | Invalid command / config | Rejected with a reason; applied state remains safe and unchanged. |
| EVT-05 | Missing reference or invalid sensor | Simulated permission LED remains off. |
| EVT-06 | Reboot with active event | Behavior matches documented persistence/reset choice. |

## MQTT and dashboard

| ID | Test | Expected result |
|---|---|---|
| NET-01 | Telemetry/schema | JSON parses; units/validity/sequence are present; telemetry rate is documented. |
| NET-02 | Remote command | ESP32 validates request and reports actual applied state; UI shows pending then applied/rejected. |
| NET-03 | Broker/Wi-Fi loss | Local acquisition, detector, and LED state continue. Dashboard indicates offline/stale. |
| NET-04 | Reconnection | Availability returns online and current state is resynchronized; no false new event from retained transient data. |
| NET-05 | Stale data | Dashboard marks telemetry stale after configured timeout. |
| NET-06 | Node-RED restart | Flow restarts cleanly; retention/history behavior matches documentation. |

## Reported metrics

At minimum report number of independent normal and perturbation runs, tested duration, number of confirmed/missed events, false alerts per unit time, detector settings, and limitations. For a classifier, include class counts and per-class precision/recall plus confusion matrix on held-out sessions. Avoid accuracy claims without a clear test set and class distribution.

## Demo sequence

1. Show startup, valid readings, and reference status.
2. Show stable normal operation and live dashboard features.
3. Apply a repeatable, safe perturbation and show candidate then confirmed event.
4. Acknowledge while the condition remains; show that the active condition is still visible.
5. Restore normal conditions and show event clear.
6. Briefly disconnect Wi-Fi/broker; show local behavior continues and dashboard marks offline/stale; reconnect and verify state recovery.