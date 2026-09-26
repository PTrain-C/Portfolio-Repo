#ifndef WIFI_CONFIG_FFT_H
#define WIFI_CONFIG_FFT_H

#include <Arduino.h>
#include <WiFiS3.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

void wifi_setup();
void wifi_tick();
int get_current_time();  // HHMM

void printCurrentNet();
void printWifiData();

#endif
