# Wokwi sensor acquisition + MQTT simulation

Standalone ESP32 DevKit v1 simulation for the equipment-monitoring project. It tests MPU6050 acquisition, a binary HR202 threshold input, an I²C LCD, status LEDs, Wi-Fi and MQTT. Root firmware is untouched.

## Simulation scope and limitations

- MPU6050/GY-521: simulated 3-axis acceleration (m/s²) and angular velocity (rad/s).
- HR202: Wokwi does not model this exact module. A pushbutton emulates its digital threshold output: released = inactive HIGH; pressed = active LOW. This tests digital input handling, not humidity calibration or the physical module's comparator.
- LCD2004: 20×4 I²C display cycles through acceleration, gyroscope and connection/health pages.
- Green LED: sensor valid and threshold inactive. Red LED: threshold active or sensor unavailable. Indicators are demonstrator outputs only.

## Pin map

| Signal | GPIO | Connection |
|---|---:|---|
| I²C SDA | 21 | MPU6050 SDA + LCD SDA |
| I²C SCL | 22 | MPU6050 SCL + LCD SCL |
| HR202 digital OUT emulator | 27 | Pushbutton, active LOW, INPUT_PULLUP |
| Green LED | 25 | LED through 330 Ω |
| Red LED | 26 | LED through 330 Ω |
| Power | 3V3/GND | MPU6050 and LCD; common ground |

Verify the exact physical ESP32 board and breakout voltage requirements before transferring the wiring. Never apply 5 V logic to ESP32 GPIO.

## Modular layout

```text
wokwi-sim/
├── platformio.ini
├── wokwi.toml
├── diagram.json
├── include/ (app_config, sensor_service, display_service, mqtt_service)
├── src/ (main, sensor_service, display_service, mqtt_service)
├── tests/test_diagram.py
└── .github/workflows/wokwi-sim-build.yml
```

## Build and run

Install PlatformIO Core or the PlatformIO VS Code extension. From this directory:

```bash
pio run
```

Open this directory as the Wokwi workspace in VS Code with the Wokwi extension and start the simulator. Wokwi loads the PlatformIO-built binary; it does not compile source itself.

## MQTT

The simulator defaults to the public HiveMQ broker at broker.hivemq.com:1883. A Wokwi virtual ESP32 cannot reach a Mosquitto broker on your PC through localhost. For a physical ESP32 on the same LAN, change MQTT_BROKER_HOST in include/app_config.h to the PC's LAN IPv4 address and configure the broker's real security settings.

Topic prefix matches the main project:
- iot/groupe6/equipement/telemetry — periodic JSON telemetry (QoS 0).
- iot/groupe6/equipement/status — retained online / Last Will offline.

The public broker is unauthenticated and shared. Use it only for non-sensitive demo telemetry; use a private authenticated/TLS broker for deployment. The MQTT client ID is derived from the ESP32 chip identifier.

Telemetry includes schema_version, device_id, seq, uptime_ms, valid, raw accelerometer/gyroscope readings, per-axis vibration-window RMS and maximum absolute deviation, and hr202_threshold_active. HR202 remains boolean; it is not reported as relative humidity.

## LCD pages

1. ACC: X/Y/Z acceleration (m/s²), validity and HR202 state.
2. GYRO: X/Y/Z angular velocity (rad/s) and MPU validity.
3. LINK: Wi-Fi, MQTT, local IP and sample sequence.

## Verification

```bash
python -m unittest discover -s tests -v
pio run
```

The Python tests validate diagram JSON, required components, connection endpoints, GPIO mapping and modular file presence. GitHub Actions runs the tests and compiles the PlatformIO firmware on pushes and pull requests. A successful compile does not prove physical wiring, broker reachability or HR202 electrical behavior; run Wokwi and then repeat hardware tests on the actual modules.

In Wokwi, move/rotate the MPU6050, press/release the HR202 emulator button, inspect the LCD/serial monitor, and subscribe to iot/groupe6/equipement/# to inspect MQTT messages. No credentials are committed.
