#include "ec_fft.h"
#include "config_fft.h"
#include <Wire.h>
#include <string.h>
#include <stdlib.h>

#define EC_DEBUG 1
#define EC_PRINT_RAW 1

char computerdata[32];
byte received_from_computer = 0;
byte serial_event = 0;
byte code = 0;
char ec_data[32];
byte in_char = 0;
byte i = 0;
int time_ = 570;  // read command processing delay in ms

char *ec;
char *tds;
char *sal;
char *sg;

float ec_float;
float tds_float;
float sal_float;
float sg_float;


void ec_setup() {
  serial_event = false;
  received_from_computer = 0;
  memset(computerdata, 0, sizeof(computerdata));
  memset(ec_data, 0, sizeof(ec_data));
  i = 0;
}

// Response CSV: EC,TDS,SAL,SG
void readnutrients() {
  computerdata[0] = 'r';
  computerdata[1] = 0;

  memset(ec_data, 0, sizeof(ec_data));
  i = 0;

  Wire.beginTransmission(I2C_ADDR_EC);
  Wire.write(computerdata);
  Wire.endTransmission();

  // Guard against accidentally waking the circuit if "sleep" was issued
  if (strcmp(computerdata, "sleep") != 0) {

    unsigned long circuitStart = millis();
    while (millis() - circuitStart < time_) {}

    Wire.requestFrom((uint8_t)I2C_ADDR_EC, (size_t)32, (bool)true);
    code = Wire.read();

    switch (code) {
      case 1:   if (EC_DEBUG) Serial.println("EC Success"); break;
      case 2:   if (EC_DEBUG) Serial.println("EC Failed"); break;
      case 254: if (EC_DEBUG) Serial.println("EC Pending"); break;
      case 255: if (EC_DEBUG) Serial.println("EC No Data"); break;
    }

    while (Wire.available()) {
      in_char = Wire.read();

      // Stop on null (EZO end-of-data) or 0xFF (empty I2C bus)
      if (in_char == 0 || in_char == 0xFF) {
        i = 0;
        break;
      }

      if (i < (sizeof(ec_data) - 1)) {
        ec_data[i] = in_char;
        i += 1;
      }
    }

    ec_data[sizeof(ec_data) - 1] = 0;

    if (EC_PRINT_RAW) {
      Serial.println("EC data:");
      Serial.println(ec_data);
      Serial.println();
    }
  }

  serial_event = false;
}

bool ec_read(float &ec_uS_cm, float &tds_ppm, float &sal_ppt, float &sg_out) {
  readnutrients();

  if (code != 1) return false;
  if (strchr(ec_data, ',') == NULL) return false;

  // strtok modifies in place, so parse a copy
  char ec_copy[32];
  strncpy(ec_copy, ec_data, sizeof(ec_copy));
  ec_copy[sizeof(ec_copy) - 1] = 0;

  char *ec_s  = strtok(ec_copy, ",");
  char *tds_s = strtok(NULL, ",");
  char *sal_s = strtok(NULL, ",");
  char *sg_s  = strtok(NULL, ",");
  if (!ec_s || !tds_s || !sal_s || !sg_s) return false;

  ec_uS_cm = atof(ec_s);
  tds_ppm  = atof(tds_s);
  sal_ppt  = atof(sal_s);
  sg_out   = atof(sg_s);
  return true;
}

void printnutrients() {
  if (code != 1) return;
  if (strchr(ec_data, ',') == NULL) return;

  char ec_copy[32];
  strncpy(ec_copy, ec_data, sizeof(ec_copy));
  ec_copy[sizeof(ec_copy) - 1] = 0;

  ec  = strtok(ec_copy, ",");
  tds = strtok(NULL, ",");
  sal = strtok(NULL, ",");
  sg  = strtok(NULL, ",");

  if (!ec || !tds || !sal || !sg) return;

  Serial.print("EC:");
  Serial.println(ec);

  Serial.print("TDS:");
  Serial.println(tds);

  Serial.print("SAL:");
  Serial.println(sal);

  Serial.print("SG:");
  Serial.println(sg);
  Serial.println();
}
