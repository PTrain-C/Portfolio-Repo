#ifndef BLYNK_CONFIG_FFT_H
#define BLYNK_CONFIG_FFT_H

// ============================================
// BLYNK CONFIGURATION
// ============================================
// These MUST be defined before including Blynk headers
// Get these values from your Blynk.Console template

#define BLYNK_TEMPLATE_ID "TMPL2SRTXx9onQ"
#define BLYNK_TEMPLATE_NAME "FFT Arduino Rev2"
#define BLYNK_AUTH_TOKEN "77RDGrajdD80yQ_yT1YNPEb1P7vBX_7h"

// Print Blynk debug info to Serial
#define BLYNK_PRINT Serial

// ============================================
// BLYNK VIRTUAL PIN ASSIGNMENTS
// ============================================
// Define which virtual pins correspond to which sensors
// This keeps all Blynk configuration in one place

#define VPIN_PH          V0  // pH sensor
#define VPIN_EC          V1  // EC (Electrical Conductivity)
#define VPIN_TDS         V2  // TDS (Total Dissolved Solids)
#define VPIN_SALINITY    V3  // Salinity
#define VPIN_SG          V4  // SG (Specific Gravity)
#define VPIN_TEMPERATURE V5  // Temperature

// Optional: control pins
/*
#define VPIN_PUMP_PH_UP    V10
#define VPIN_PUMP_PH_DOWN  V11
#define VPIN_PUMP_EC_UP    V12
#define VPIN_PUMP_EC_DOWN  V13

// Optional: status indicators
#define VPIN_SYSTEM_STATUS V20
#define VPIN_LAST_DOSE     V21
*/
#endif
