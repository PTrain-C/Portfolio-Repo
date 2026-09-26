#include "dosing_fft.h"

#include "config_fft.h"
#include "ph_fft.h"
#include "ec_fft.h"
#include "pmp_fft.h"
#include "lights_fft.h"

// ----------------------------------------------------------------------
// Control order: EC first, then pH (per FFT 2026 spec).
// pH and EC control are INDEPENDENT: if one sensor fails, the other
// side still runs. Only skips a cycle if both sensors fail.
// ----------------------------------------------------------------------

static bool within_deadband(float x, float target, float deadband) {
  return (x >= target - deadband) && (x <= target + deadband);
}

static void print_readings(float pH, float ec_uS, bool pH_valid, bool ec_valid) {
  if (!PRINT_SENSOR_READINGS) return;
  Serial.print("  pH=");
  if (pH_valid) Serial.print(pH, 2); else Serial.print("FAIL");
  Serial.print("  EC=");
  if (ec_valid) { Serial.print(ec_uS, 0); Serial.print("uS"); }
  else Serial.print("FAIL");
  Serial.println();
}

// Reads both sensors. Returns bitmask: bit0 = pH ok, bit1 = EC ok.
static int read_sensors(float &pH, float &ec_uS, bool &pH_valid, bool &ec_valid) {
  float tds, sal, sg;
  pH_valid = ph_read(pH);
  ec_valid = ec_read(ec_uS, tds, sal, sg);
  return (pH_valid ? 1 : 0) | (ec_valid ? 2 : 0);
}

static void mix() {
  unsigned long t0 = millis();
  while (millis() - t0 < MIXING_TIME_MS) {
    lights_tick();
  }
}

void dosing_setup() {
  ph_setup();
  ec_setup();
  pmp_setup();
}

void dosing_tick() {
  Serial.println("--- Dosing cycle ---");

  float pH = NAN, ec_uS = NAN;
  bool pH_valid = false, ec_valid = false;
  int status = read_sensors(pH, ec_uS, pH_valid, ec_valid);
  print_readings(pH, ec_uS, pH_valid, ec_valid);

  if (status == 0) {
    Serial.println("  Both sensors failed, skipping cycle");
    return;
  }

  // --------------------------
  // EC CONTROL (first)
  // --------------------------
  if (ec_valid) {
    int pulses = 0;
    while (!within_deadband(ec_uS, EC_TARGET, EC_DEADBAND) && pulses < MAX_EC_PULSES_PER_CYCLE) {
      if (ec_uS < EC_TARGET - EC_DEADBAND) {
        if (PRINT_DOSING_ACTIONS) Serial.println("  EC low, dosing UP");
        pmp_dose_ml(I2C_ADDR_PMP_EC_UP, PULSE_ML_EC);
      } else {
        if (I2C_ADDR_PMP_EC_DOWN == 0) {
          if (PRINT_DOSING_ACTIONS) Serial.println("  EC high, no EC_DOWN pump configured");
          break;
        }
        if (PRINT_DOSING_ACTIONS) Serial.println("  EC high, dosing DOWN");
        pmp_dose_ml(I2C_ADDR_PMP_EC_DOWN, PULSE_ML_EC);
      }
      pulses++;
      mix();
      read_sensors(pH, ec_uS, pH_valid, ec_valid);
      print_readings(pH, ec_uS, pH_valid, ec_valid);
      if (!ec_valid) break;
    }
  }

  // --------------------------
  // pH CONTROL (after EC)
  // --------------------------
  if (pH_valid) {
    int pulses = 0;
    while (!within_deadband(pH, PH_TARGET, PH_DEADBAND) && pulses < MAX_PH_PULSES_PER_CYCLE) {
      if (pH > PH_TARGET + PH_DEADBAND) {
        if (PRINT_DOSING_ACTIONS) Serial.println("  pH high, dosing DOWN");
        pmp_dose_ml(I2C_ADDR_PMP_PH_DOWN, PULSE_ML_PH);
      } else {
        if (PRINT_DOSING_ACTIONS) Serial.println("  pH low, dosing UP");
        pmp_dose_ml(I2C_ADDR_PMP_PH_UP, PULSE_ML_PH);
      }
      pulses++;
      mix();
      read_sensors(pH, ec_uS, pH_valid, ec_valid);
      print_readings(pH, ec_uS, pH_valid, ec_valid);
      if (!pH_valid) break;
    }
  }

  Serial.println("  Cycle done");
}
