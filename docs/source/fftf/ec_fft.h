#ifndef EC_FFT_H
#define EC_FFT_H

#include <Arduino.h>

void ec_setup();
void readnutrients();
void printnutrients();

// Reads + parses CSV into four floats. Returns true on success.
bool ec_read(float &ec_uS_cm, float &tds_ppm, float &sal_ppt, float &sg);

#endif
