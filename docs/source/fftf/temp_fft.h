#ifndef TEMP_FFT_H
#define TEMP_FFT_H

#include <Arduino.h>

void temp_setup();
void readTemperature();
void printTemperature();

// Convenience API for control loops: performs a read + parse into a float.
// Returns true only when the temperature circuit returns a success code and parsing succeeds.
bool temp_read(float &temp_celsius);

#endif
