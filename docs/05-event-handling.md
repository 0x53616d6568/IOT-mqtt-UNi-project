# Event Handling and Local State

## Principles

- Decisions and indicators run locally on the ESP32; MQTT loss must not disable them.
- Startup, missing reference, invalid sensor input, or stale internal data must never be represented as normal.
- Acknowledgement is not the same as clearing an active condition.
- Dashboard commands request changes; `/state` confirms what the ESP32 actually applied.
- LEDs are demonstrator indicators only, not safety-rated outputs.

## Suggested states

| State | Meaning | Green LED | Red LED |
|---|---|---:|---:|
| `STARTUP` | Waiting for sensor checks and valid windows | Off | Off |
| `NO_REFERENCE` | Sensor is available but normal reference is absent/invalid | Off | Off |
| `NORMAL` | Valid reference; no confirmed vibration anomaly; HR202 not active | On | Off |
| `VIGILANCE` | Features cross warning criteria but confirmation has not elapsed | Off or brief policy-defined indicator | Off |
| `ANOMALY_ACTIVE` | Vibration anomaly confirmed and not yet cleared | Off | On |
| `HUMIDITY_THRESHOLD_ACTIVE` | HR202 digital threshold condition is active | Off (policy-defined) | On |
| `SENSOR_INVALID` | Missing/out-of-range/stale local sensor data | Off | On or distinct error pattern, explicitly documented |
| `DISARMED` | Optional monitoring disabled; measurements may continue, no operational permission | Off | Off unless a latched event must remain indicated |

If vibration anomaly and HR202 are active together, preserve both causes in the event list; do not collapse their history into a generic alarm.

## Event lifecycle

```text
candidate -> confirmed/active -> acknowledged (optional) -> cleared -> closed
```

- **Candidate:** threshold/model condition begins. Track duration/windows and suppress single-window noise.
- **Confirmed/active:** confirmation criterion is met. Create a unique event ID, cause, onset time/uptime, relevant features, detector/model version, and publish `/alerts`.
- **Acknowledged:** operator acknowledges the event. Record user/operator label and time if available. Acknowledgement may silence a simulated annunciator only if configured, but the condition remains visibly active.
- **Cleared:** condition remains below clear criteria for a separate clear interval/hysteresis. Publish a clear transition; retain the event in history.
- **Closed:** active event has been cleared and acknowledged if workflow requires it.

## Transitions and edge cases

- On boot, outputs remain off until sensor input and required reference are validated.
- Missing reference prevents run-permission indication and should surface as `NO_REFERENCE`.
- Invalid measurement produces `SENSOR_INVALID`; it must not count as a normal window or create false normal statistics.
- Acknowledging a persistent active condition does not clear it. The UI continues to show `active=true` until the clear rule is met.
- On MQTT disconnect, continue local event logic; mark availability offline. On reconnect, publish current state and active events or an explicit resynchronization snapshot.
- On reboot, decide whether active events are restored from nonvolatile memory or reset. Document the choice; never silently claim event history was preserved if it was not.
- Commands with unsupported mode, missing fields, or unsafe values are rejected and reported.

## Configuration to determine experimentally

- Candidate threshold and warning state.
- Confirmation count/duration and clear hysteresis/duration.
- Whether acknowledgement changes only display state or also simulated buzzer/LED behavior.
- Whether events survive reboot and how event IDs remain unique.
- HR202 active polarity and event debounce.