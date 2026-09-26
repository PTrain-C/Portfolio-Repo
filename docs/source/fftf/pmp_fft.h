#ifndef PMP_FFT_H
#define PMP_FFT_H

#include <Arduino.h>

void pmp_setup();

// Send raw ASCII command to pump at i2c_addr. Returns true on response code 1.
bool pmp_send_cmd(uint8_t i2c_addr, const char *cmd, char *response_buf, size_t response_len);

// Dose a volume in mL via "d,<ml>".
bool pmp_dose_ml(uint8_t i2c_addr, float ml);

// Stop via "X" (firmware dependent).
bool pmp_stop(uint8_t i2c_addr);

#endif
