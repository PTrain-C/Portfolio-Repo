#include "ph_fft.h"
#include "config_fft.h"
#include <Wire.h>
#include <string.h>
#include <stdlib.h>

#define PH_DEBUG 1
#define PH_PRINT_RAW 1

char ph_computerdata[20];
byte ph_received_from_computer = 0;
byte ph_code = 0;
char ph_data[32];
byte ph_in_char = 0;
byte ph_i = 0;
int ph_time_ = 815;  // read command processing delay in ms

void ph_setup() {
  memset(ph_computerdata, 0, sizeof(ph_computerdata));
  memset(ph_data, 0, sizeof(ph_data));
  ph_received_from_computer = 0;
  ph_code = 0;
  ph_i = 0;
}

void readpH() {
  ph_computerdata[0] = 'r';
  ph_computerdata[1] = 0;

  memset(ph_data, 0, sizeof(ph_data));
  ph_i = 0;

  Wire.beginTransmission(I2C_ADDR_PH);
  Wire.write(ph_computerdata);
  Wire.endTransmission();

  unsigned long readStart = millis();
  while (millis() - readStart < ph_time_) {}

  Wire.requestFrom((int)I2C_ADDR_PH, 32, (int)1);
  if (!Wire.available()) {
    ph_code = 255;
    return;
  }

  ph_code = Wire.read();

  switch (ph_code) {
    case 1:   if (PH_DEBUG) Serial.println("pH Success"); break;
    case 2:   if (PH_DEBUG) Serial.println("pH Failed"); break;
    case 254: if (PH_DEBUG) Serial.println("pH Pending"); break;
    case 255: if (PH_DEBUG) Serial.println("pH No Data"); break;
  }

  while (Wire.available()) {
    ph_in_char = Wire.read();

    // Stop on null (EZO end-of-data) or 0xFF (empty I2C bus)
    if (ph_in_char == 0 || ph_in_char == 0xFF) {
      ph_i = 0;
      break;
    }

    if (ph_i < (sizeof(ph_data) - 1)) {
      ph_data[ph_i] = (char)ph_in_char;
      ph_i++;
    }
  }

  ph_data[sizeof(ph_data) - 1] = 0;

  if (PH_PRINT_RAW) {
    Serial.println("pH data:");
    Serial.println(ph_data);
    Serial.println();
  }
}

void printpH() {
  if (ph_code != 1) return;
  if (ph_data[0] == 0) return;

  Serial.print("pH:");
  Serial.println(ph_data);
  Serial.println();
}

bool ph_read(float &pH) {
  readpH();

  if (ph_code != 1) return false;
  if (ph_data[0] == 0) return false;

  pH = (float)atof(ph_data);

  if (pH < 0.0f || pH > 14.0f) return false;

  return true;
}
