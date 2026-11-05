# Schematic and Wiring Plan

> **Do not wire from this document alone.** It is a planning checklist, not a board-specific schematic. Confirm the exact ESP32 board, module revision, pin labels, power requirements, and logic levels first. Fill every `TBD` only after verification.

## Planned connections

| Device signal | ESP32 connection | Notes / verification |
|---|---|---|
| MPU6050 SDA | TBD | I²C data; choose a supported GPIO and record it. |
| MPU6050 SCL | TBD | I²C clock; verify pull-ups and logic level. |
| MPU6050 VCC/GND | TBD / common GND | Check the breakout's allowed input supply and onboard regulation. |
| HR202 digital OUT | TBD GPIO input | Verify output voltage is safe for ESP32 (3.3 V max at input) and determine active-high/active-low polarity. |
| HR202 VCC/GND | TBD / common GND | Confirm module requirements; do not assume every board is 3.3 V compatible. |
| Green LED GPIO | TBD | Use a series resistor (brief specifies 330 Ω); LED is a simulated permission indicator. |
| Red LED GPIO | TBD | Use its own series resistor (brief specifies 330 Ω); simulated alarm indicator. |
| USB | ESP32 board | Use the board's recommended power input. |

## Planning diagram (logical only)

```text
                 I2C SDA/SCL (TBD)        +--> green LED + 330 Ω (simulated permission)
MPU6050/GY-521 <----------------------> ESP32 +--> red LED + 330 Ω (simulated alarm)
HR202 digital OUT (verified 3.3 V) ---> GPIO
All module grounds --------------------> common ground
ESP32 USB -----------------------------> PC / suitable USB supply
```

## Before powering

- [ ] Identify exact board model and consult its pinout. Avoid boot-strapping, flash, input-only/output-only, and USB-reserved pins where applicable.
- [ ] Confirm both sensor modules' supply ranges and whether the breakout includes regulators or level shifting.
- [ ] Confirm SDA/SCL pull-up voltage is compatible with ESP32; do not connect 5 V logic to ESP32 GPIO.
- [ ] Measure/verify HR202 OUT at the ESP32 input under both threshold states before final connection.
- [ ] Establish common ground between ESP32 and modules.
- [ ] Wire LED with a series resistor in the correct orientation; never connect an LED directly across GPIO and GND.
- [ ] Inspect for shorts and confirm power-source current capability.

## Schematic capture TODO

- [ ] Add actual board/module names and revision.
- [ ] Add exact GPIO assignments, I²C address, resistor values, and power rails.
- [ ] Add sensor mounting orientation and a photo/diagram showing the vibration fixture.
- [ ] Label all ground connections and logic voltage domains.
- [ ] Validate the final diagram against the physical prototype and update it after any change.