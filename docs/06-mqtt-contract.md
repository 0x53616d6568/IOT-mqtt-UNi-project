# MQTT Contract

## Broker and prefix

- Broker: Mosquitto on the PC; ESP32 connects to the PC's LAN IP, not `localhost`.
- Prefix: `iot/groupe6/equipement`
- Use UTF-8 JSON payloads. Publish at a dashboard-friendly rate; acquire and analyze vibration faster locally than telemetry publication.
- Retain availability and current state only where stated below. Do not retain transient event messages as if they were new events on every subscription.
- Credentials belong in a local untracked configuration file, never in this repository.

## Topics

| Topic | Direction | Purpose | Suggested QoS / retained |
|---|---|---|---|
| `/telemetry` | ESP32 → broker | Periodic features, HR202, reference and validity | QoS 0; not retained |
| `/state` | ESP32 → broker | Confirmed applied mode/config/output/event summary | QoS 1; retained snapshot |
| `/cmd` | dashboard → ESP32 | Request mode, reference capture, start/stop simulation, acknowledge | QoS 1; not retained |
| `/config/set` | dashboard → ESP32 | Request thresholds, timings, telemetry period | QoS 1; not retained |
| `/alerts` | ESP32 → broker | Event created/updated/cleared transitions | QoS 1; not retained; unique event ID |
| `/status` | ESP32 → broker | `online` / `offline`; configure MQTT Last Will as `offline` | QoS 1; retained |

QoS and retained behavior are project defaults; verify that the broker and clients implement them as intended.

## Payload conventions

- Include `schema_version` and `device_id` in every payload.
- Include monotonic `uptime_ms`; wall-clock UTC may be included only when time is synchronized.
- Use `null` for unavailable measurements and explicit validity/error fields; never encode missing sensor data as zero.
- Use SI units where applicable. Document the accelerometer units and range.
- The HR202 value is boolean/threshold state, not `%RH`.
- Include `seq` for telemetry ordering and gap detection.

## Example telemetry

```json
{
  "schema_version": 1,
  "device_id": "esp32-equip-01",
  "seq": 1042,
  "uptime_ms": 392100,
  "valid": true,
  "features": {
    "rms_x": 0.012,
    "rms_y": 0.009,
    "rms_z": 0.018,
    "max_abs_x": 0.041,
    "max_abs_y": 0.029,
    "max_abs_z": 0.057
  },
  "hr202_threshold_active": false,
  "reference_valid": true,
  "detector": {"type": "threshold", "version": "baseline-01", "score": 0.18}
}
```

Feature values above are illustrative only; set real units/scale in the firmware and schema before implementation.

## Example state

```json
{
  "schema_version": 1,
  "device_id": "esp32-equip-01",
  "uptime_ms": 392200,
  "mode": "AUTO",
  "state": "NORMAL",
  "reference_valid": true,
  "green_permission_led": true,
  "red_alarm_led": false,
  "active_event_ids": []
}
```

## Example command and acknowledgement

```json
{"schema_version":1,"command_id":"cmd-0007","action":"capture_reference"}
```

The ESP32 must validate the command, execute only if prerequisites are met, and publish actual resulting state to `/state`. For command-level response, either publish a command result field in state or define a separate response topic only if needed. The dashboard must show pending/rejected/applied distinctly.

## Example event

```json
{
  "schema_version": 1,
  "device_id": "esp32-equip-01",
  "event_id": "esp32-equip-01-0003",
  "uptime_ms": 414200,
  "cause": "vibration_anomaly",
  "status": "confirmed",
  "active": true,
  "acknowledged": false,
  "features": {"rms_x": 0.031, "max_abs_x": 0.12},
  "detector_version": "xgb-01"
}
```

All numbers are examples, not calibrated thresholds or device readings.

## Commands to support

- `capture_reference` (only when sensor data is valid and operating condition is declared suitable)
- `set_mode` with allowed values documented by firmware (for example `AUTO`, `DISARMED`)
- `acknowledge_event` with an event ID
- `set_config` or `/config/set` updates, with range checks and explicit applied/rejected result

Do not offer remote control of real machinery; this prototype only controls indicator LEDs.