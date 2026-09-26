#ifndef PH_FFT_H
#define PH_FFT_H

#include <Arduino.h>

void ph_setup();
void readpH();
void printpH();

// Reads + parses into a float. Returns true on success + valid range.
bool ph_read(float &pH);

#endif
