# Node-RED Dashboard

## Dashboard purpose

Show measurements and confirmed device behavior clearly. The dashboard is a monitoring/control surface for the prototype; it is not a safety system. Use the ESP32's `/state` as the source of truth for applied outputs and mode.

## Recommended sections

### 1. Connection and device health

- MQTT/ESP32 status: online/offline from `/status`.
- Last telemetry time and stale-data indicator based on a configured timeout.
- Sequence gap indicator, firmware version, sensor validity, and reference validity.
- Distinguish broker connectivity from sensor health.

### 2. Vibration

- Time series for per-axis RMS and maximum amplitude; label units and window duration.
- Current detector type/version, score if applicable, and state: normal / vigilance / anomaly / invalid.
- Baseline/reference status and when captured.
- Avoid charting raw high-rate samples through MQTT unless there is a deliberate bandwidth plan.

### 3. HR202 threshold

- Show a binary label such as `threshold active` / `inactive`.
- Do not display a numeric humidity percentage for this module.

### 4. Events

- Active event banner with cause, onset, duration, acknowledged status, and current/cleared state.
- Event journal with event ID, cause, state transitions, timestamps, and relevant feature summary.
- Separate vibration anomalies, HR202 threshold events, sensor invalidity, and connectivity events.

### 5. Controls

- Capture/reset reference (disabled unless sensor is valid and capture prerequisites are satisfied).
- Mode selection if implemented.
- Acknowledge event by event ID.
- Threshold/timing configuration only with validation and visible applied/rejected result.
- Every control should show `pending`, `applied`, or `rejected`; never optimistically label a command as applied.

### 6. Simulated indicators

- Display green and red LED state from `/state`, not from command intent.
- Label as simulated permission/alarm indicators, not actual motor control.

## Data-flow checks

- Subscribe to `iot/groupe6/equipement/#` during integration and verify each topic against `docs/06-mqtt-contract.md`.
- Parse JSON with error handling. Malformed payloads should appear as a diagnostic, not crash a flow.
- Use persistent flow context or storage only if configured and documented; otherwise note that the event history resets on Node-RED restart.
- Import/export the Node-RED flow JSON and record required dashboard nodes/versions. Do not commit passwords or broker secrets.

## Acceptance checks

- Offline, stale, invalid, and no-reference states are visually distinct from normal.
- Threshold scores/charts have units and the feature window is stated.
- Acknowledgement leaves a persistent active fault visible until the device reports clear.
- Restarting Node-RED does not falsely show retained transient alerts as new events.
- The dashboard reflects the ESP32's confirmed `/state` after every command.