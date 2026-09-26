#include "pmp_fft.h"
#include <Wire.h>
#include <string.h>
#include <stdlib.h>

// Datasheet minimum is 318 ms for dispense commands; 350 ms gives a small safety margin.
static constexpr int PMP_DELAY_MS = 350;

#define PMP_DEBUG 1

void pmp_setup() {
  // Serial.begin and Wire.begin are called in the main .ino
}

bool pmp_send_cmd(uint8_t i2c_addr, const char *cmd, char *response_buf, size_t response_len) {
  if (response_buf && response_len > 0) {
    memset(response_buf, 0, response_len);
  }

  Wire.beginTransmission(i2c_addr);
  Wire.write(cmd);
  uint8_t tx_res = Wire.endTransmission();
  if (tx_res != 0) {
    if (PMP_DEBUG) {
      Serial.print("PMP I2C TX error: ");
      Serial.println(tx_res);
    }
    return false;
  }

  unsigned long pumpStart = millis();
  while (millis() - pumpStart < PMP_DELAY_MS) {}

  Wire.requestFrom((int)i2c_addr, 20, (int)1);
  if (!Wire.available()) return false;

  uint8_t code = Wire.read();

  if (PMP_DEBUG) {
    Serial.print("PMP code: ");
    Serial.println(code);
  }

  size_t i = 0;
  while (Wire.available()) {
    char c = (char)Wire.read();
    if (response_buf && i < response_len - 1) {
      response_buf[i++] = c;
    }
    if (c == 0) break;
  }
  if (response_buf && response_len > 0) response_buf[response_len - 1] = 0;

  return (code == 1);
}

bool pmp_dose_ml(uint8_t i2c_addr, float ml) {
  // Atlas accepts at most 2 decimal places. "d,0.200" returns syntax error.
  char num[12];
  dtostrf(ml, 0, 2, num);

  // EZO-PMP I2C mode uses lowercase "d,". Uppercase returns code 2.
  char full_cmd[20];
  snprintf(full_cmd, sizeof(full_cmd), "d,%s", num);
  full_cmd[sizeof(full_cmd) - 1] = 0;

  if (PMP_DEBUG) {
    Serial.print("PMP sending cmd: \"");
    Serial.print(full_cmd);
    Serial.print("\" to addr ");
    Serial.println(i2c_addr);
  }

  char resp[20];
  return pmp_send_cmd(i2c_addr, full_cmd, resp, sizeof(resp));
}

bool pmp_stop(uint8_t i2c_addr) {
  char resp[20];
  return pmp_send_cmd(i2c_addr, "X", resp, sizeof(resp));
}
