#include "temp_fft.h"
#include "config_fft.h"
#include <Wire.h>
#include <string.h>
#include <stdlib.h>

// Set to 1 to print I2C status codes every read (spammy in always-read mode)
#define TEMP_DEBUG 1
// Set to 1 to print the raw temperature string every read (spammy)
#define TEMP_PRINT_RAW 1

// --- Module-level state ---
char temp_data[20];
byte temp_code = 0;
int temp_time_ = 600;  // Temperature sensor needs ~600ms

// ---------------------------------------------------------------------------
// temp_setup()
// Called once from setup(). Clears buffers and resets state.
// Expects Wire.begin() and Serial.begin() to already be called in the .ino.
// ---------------------------------------------------------------------------
void temp_setup() {
  memset(temp_data, 0, sizeof(temp_data));
  temp_code = 0;
}

// ---------------------------------------------------------------------------
// readTemperature()
// Sends the "R" (read) command to the temperature circuit over I2C, waits
// for the processing delay (~600ms), then reads the response into temp_data[].
//
// BUGFIX (2026): 0xFF bytes are treated as end-of-data. The I2C bus returns
// 0xFF when there is nothing left to read, but the original code only broke
// on 0x00 (null). This could fill the buffer with garbage.
// ---------------------------------------------------------------------------
void readTemperature() {
  memset(temp_data, 0, sizeof(temp_data));

  Wire.beginTransmission(I2C_ADDR_TEMP);
  Wire.write("R");
  Wire.endTransmission();

  // Temperature sensor requires ~600ms
  unsigned long tempStart = millis();
  while (millis() - tempStart < temp_time_) {}

  Wire.requestFrom((int)I2C_ADDR_TEMP, 20, (int)1);
  if (!Wire.available()) {
    temp_code = 255;
    return;
  }

  temp_code = Wire.read();

  switch (temp_code) {
    case 1:
      if (TEMP_DEBUG) Serial.println("Temp Success");
      break;
    case 2:
      if (TEMP_DEBUG) Serial.println("Temp Failed");
      break;
    case 254:
      if (TEMP_DEBUG) Serial.println("Temp Pending");
      break;
    case 255:
      if (TEMP_DEBUG) Serial.println("Temp No Data");
      break;
  }

  int i = 0;
  while (Wire.available()) {
    char temp_in_char = Wire.read();

    // 0x00 = null terminator from device, 0xFF = no more I2C data.
    // Both mean "stop reading."
    if (temp_in_char == 0 || (uint8_t)temp_in_char == 0xFF) {
      i = 0;
      break;
    }

    if (i < (int)(sizeof(temp_data) - 1)) {
      temp_data[i] = temp_in_char;
      i++;
    }
  }

  temp_data[sizeof(temp_data) - 1] = 0;  // Force null termination

  if (TEMP_PRINT_RAW) {
    Serial.println("Temp data:");
    Serial.println(temp_data);
    Serial.println();
  }
}

// ---------------------------------------------------------------------------
// printTemperature()
// Prints the last successful temperature reading to Serial.
// ---------------------------------------------------------------------------
void printTemperature() {
  if (temp_code != 1) return;
  if (temp_data[0] == 0) return;

  Serial.print("Temperature:");
  Serial.println(temp_data);
  Serial.println();
}

// ---------------------------------------------------------------------------
// temp_read()
// High-level API for control loops.
// Calls readTemperature(), then parses the response into a float.
// Returns true only on success with a sane value (-50 to 150 C).
// ---------------------------------------------------------------------------
bool temp_read(float &temp_celsius) {
  readTemperature();

  if (temp_code != 1) return false;
  if (temp_data[0] == 0) return false;

  temp_celsius = (float)atof(temp_data);
  // Sanity check
  if (temp_celsius < -50.0f || temp_celsius > 150.0f) return false;

  return true;
}
