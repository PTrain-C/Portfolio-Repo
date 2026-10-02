/*
  FarmsForThought -- FFT 2026 main sketch
  All configurable values live in config_fft.h.
*/

#include <Wire.h>
#include "config_fft.h"
#include "wifi_config_fft.h"
#include "lights_fft.h"
#include "ph_fft.h"
#include "ec_fft.h"
#include "pmp_fft.h"
#include "dosing_fft.h"
#include "temp_fft.h"

// Timing for the main read/dose loop
unsigned long currMillis = 0;
unsigned long prevMillis = 0;
const unsigned long READ_INTERVAL = 10000;  // 10 seconds between dosing cycles

void setup() {
  Serial.begin(9600);
  while (!Serial) { delay(10); }

  // --- I2C bus ---
  // Called once here; all sensor/pump modules expect it to be ready.
  Wire.begin();
  Serial.println("I2C enabled");

  // --- WiFi + NTP ---
  wifi_setup();
  Serial.println("WiFi enabled");

  // --- Water pump (always on) ---
  pinMode(WATERPUMP_OUTPUT_PIN, OUTPUT);
  digitalWrite(WATERPUMP_OUTPUT_PIN, HIGH);

  // --- Float switch (active-low, uses internal pullup) ---
  pinMode(FLOAT_SWITCH_PIN, INPUT_PULLUP);
  digitalRead(FLOAT_SWITCH_PIN);

  // Air pump (always on)
  pinMode(AIR_PUMP_PIN, OUTPUT);
  digitalWrite(AIR_PUMP_PIN, HIGH);
  Serial.println("Air Pump ON");

  // --- Dosing system (pH + EC sensors, pump wrappers) ---
  dosing_setup();
  Serial.println("Dosing enabled");

  // --- Temperature sensor ---
  temp_setup();
  Serial.println("Temp sensor enabled");

  light_setup();

  Serial.println("FFT 2026 boot complete");
  Serial.println();
}

void loop() {
  currMillis = millis();

  // Keep NTP time updated
  wifi_tick();

  // Light scheduling
  lights_tick();

  // Run dosing cycle every READ_INTERVAL ms
  if (currMillis - prevMillis > READ_INTERVAL) {
    prevMillis = currMillis;
    dosing_tick();
  }
}
