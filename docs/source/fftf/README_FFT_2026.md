# FarmsForThought FFT 2026

Hydroponic control system built on an Arduino Uno WiFi Rev2. Handles pH, EC, temperature, grow lights, water pump, float switch, and Atlas EZO-PMP dosing pumps, all over I2C.

---

## File map

| File | Purpose |
|---|---|
| `farmsforthoughtcode_fft_2026.ino` | Main sketch. Initializes Serial and Wire, then runs the tick functions in `loop()`. |
| `config_fft.h` | Single source of truth for pins, I2C addresses, setpoints, and tuning values. |
| `wifi_config_fft.{h,cpp}` | WiFiS3 connection and NTP time client. |
| `ph_fft.{h,cpp}` | Atlas EZO pH read over I2C. |
| `ec_fft.{h,cpp}` | Atlas EZO EC read over I2C. Parses the four-field CSV output. |
| `temp_fft.{h,cpp}` | Atlas EZO temperature read over I2C. Not currently used by the control loop. |
| `pmp_fft.{h,cpp}` | Atlas EZO-PMP I2C wrapper. Sends `d,<ml>` dose commands. |
| `lights_fft.{h,cpp}` | millis-based on/off light cycle. No NTP dependency. |
| `dosing_fft.{h,cpp}` | Closed-loop controller. EC stabilizes first, then pH. |

---

## Default I2C addresses

| Device | Dec | Hex |
|---|---|---|
| pH | 99 | 0x63 |
| EC | 100 | 0x64 |
| Temp | 102 | 0x66 |
| pH down pump | 103 | 0x67 |
| pH up pump | 104 | 0x68 |
| EC up pump | 105 | 0x69 |
| EC down pump | 106 | 0x6A |

Run an I2C scanner to confirm each device before deploying. Set `I2C_ADDR_PMP_EC_DOWN` to `0` in config to disable the dilution pump entirely.

---

## Dosing behavior

Every `CONTROL_INTERVAL_MS` (10 s default):

1. Read pH and EC. If both fail, skip the cycle.
2. **EC loop:** if outside `EC_TARGET +/- EC_DEADBAND`, pulse `PULSE_ML_EC` mL from the appropriate pump, wait `MIXING_TIME_MS`, re-read. Stop when in band or after `MAX_EC_PULSES_PER_CYCLE` attempts.
3. **pH loop:** same structure, using `PULSE_ML_PH`, `PH_TARGET`, `PH_DEADBAND`, and `MAX_PH_PULSES_PER_CYCLE`.

EC runs first because nutrient concentration changes push pH around. Locking EC first keeps the pH loop from chasing a moving target.

pH and EC are independent: if one sensor read fails, the other side still runs. The cycle is only skipped when both fail.

---

## Things to verify in `config_fft.h` before deploying

- `ssid` and `pass` (currently open USC Guest Wireless, password is empty)
- `TIMEZONE_OFFSET_SEC` (PST = -28800, PDT = -25200)
- `LIGHT_OUTPUT_PIN`, `WATERPUMP_OUTPUT_PIN`, `FLOAT_SWITCH_PIN`
- All seven I2C addresses (run the scanner)
- `PH_TARGET`, `PH_DEADBAND`
- `EC_TARGET`, `EC_DEADBAND` (units must match what the Atlas EC circuit outputs, usually uS/cm)
- `PULSE_ML_PH`, `PULSE_ML_EC`
- `MAX_PH_PULSES_PER_CYCLE`, `MAX_EC_PULSES_PER_CYCLE`

---

## Atlas EZO notes

Per-command processing delays are already baked into the drivers:

| Command | Delay |
|---|---|
| pH read | 815 ms |
| EC read | 570 ms |
| Temp read | 600 ms |
| PMP dispense | 350 ms (datasheet minimum 318 ms) |

**EC output configuration.** The CSV parser expects all four outputs enabled. Send these once over I2C; they persist in EEPROM:

```
O,EC,1
O,TDS,1
O,SAL,1
O,SG,1
```

**EZO-PMP command quirks.** I2C mode expects the dose command as lowercase `d,<ml>`. Uppercase `D,<ml>` returns code 2. Volumes must have at most two decimal places: `d,0.20` works, `d,0.200` fails with a syntax error.

**I2C read loops must break on both `0x00` and `0xFF`.** `0x00` is the EZO end-of-data terminator. `0xFF` is what the I2C bus returns once the device has no more data to send. Missing the `0xFF` case lets garbage bytes into the buffer and silently corrupts readings. This fix is applied in the pH, EC, and temp drivers.

---

## Boot sequence

1. `Serial.begin(9600)`
2. `Wire.begin()`
3. `wifi_setup()` connects WiFi and starts NTP.
4. `light_setup()` initializes the light output pin.
5. `dosing_setup()` initializes the pH, EC, and pump modules.

## Runtime loop

- `wifi_tick()` updates NTP.
- `lights_tick()` toggles the light pin based on the millis-based schedule.
- `dosing_tick()` runs one dosing cycle.

---

## Current status

- Blynk fully removed from the codebase.
- Dosing order is EC first, then pH.
- 0xFF stop-byte fix applied to pH, EC, and temperature drivers.
- Lights and temperature modules are in the codebase but pending updates in a later pass.

---

## Quick start checklist

1. Edit `config_fft.h` (WiFi, timezone, pins, I2C addresses, setpoints).
2. Wire I2C correctly (SDA, SCL, common ground).
3. Confirm each device responds at the expected address with an I2C scanner.
4. Flash `farmsforthoughtcode_fft_2026.ino`.
5. Watch Serial:
   - WiFi connects, time updates
   - pH and EC reads print values
   - dosing actions print only when out of band
