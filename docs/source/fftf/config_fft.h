#ifndef CONFIG_FFT_H
#define CONFIG_FFT_H

// =============================================
// config_fft.h -- Single source of truth
// =============================================
// All hardware pins, I2C addresses, setpoints,
// and tuning values live here. Edit this file
// before deploying to real hardware.

// -----------------------------
// WiFi credentials
// -----------------------------
// For open networks (no password), leave pass as "".
// wifi_config_fft.cpp detects empty pass and uses
// the single-arg WiFi.begin(ssid) automatically.

static const char ssid[] = "USC Guest Wireless";
static const char pass[] = "";

// -----------------------------
// Timezone (seconds offset from UTC)
// -----------------------------
// PST = -28800, PDT = -25200

static constexpr long TIMEZONE_OFFSET_SEC = -28800;

// -----------------------------
// Light control
// -----------------------------

static constexpr int LIGHT_1_PIN = 2;
static constexpr int LIGHT_2_PIN = 3;
static constexpr int LIGHT_3_PIN = 4;

// 8 hours on / 8 hours off
static constexpr unsigned long LIGHT_ON_DURATION_MS  = 10UL * 1000UL;  // 8 hours
static constexpr unsigned long LIGHT_OFF_DURATION_MS = 10UL * 1000UL;  // 8 hours
// 10UL * 1000UL for 10s on and 10s off
// 8UL * 60UL * 60UL * 1000UL for 8 hours on 8 hours off

// -----------------------------
// Water pump + float switch
// -----------------------------

static constexpr int WATERPUMP_OUTPUT_PIN = 7;
static constexpr int FLOAT_SWITCH_PIN     = 8;
static constexpr int AIR_PUMP_PIN         = 9;

// -----------------------------
// Atlas EZO I2C addresses
// -----------------------------
// Confirm with the I2C scanner sketch before deploying.
// Default addresses: pH = 99 (0x63), EC = 100 (0x64)

static constexpr uint8_t I2C_ADDR_PH   = 99;          // pH EZO circuit
static constexpr uint8_t I2C_ADDR_EC   = 100;         // EC EZO circuit
static constexpr uint8_t I2C_ADDR_TEMP = 102;         // Temperature EZO circuit

// Dosing pumps -- replace with your actual pump addresses
static constexpr uint8_t I2C_ADDR_PMP_PH_UP   = 104; // 0x68
static constexpr uint8_t I2C_ADDR_PMP_PH_DOWN = 103; // 0x67
static constexpr uint8_t I2C_ADDR_PMP_EC_UP   = 105; // 0x69
static constexpr uint8_t I2C_ADDR_PMP_EC_DOWN = 106;  // 0x6A  (set to 0 to disable dilution)

// -----------------------------
// Dosing setpoints
// -----------------------------
// EC units must match what the Atlas EC circuit outputs
// (usually uS/cm unless you've reconfigured it).

static constexpr float PH_TARGET   = 7.00f;
static constexpr float PH_DEADBAND = 0.50f;

static constexpr float EC_TARGET   = 1500.0f;        // TODO: set for your crop
static constexpr float EC_DEADBAND = 50.0f;           // TODO: tune

// -----------------------------
// Dosing behavior
// -----------------------------
// Each dosing cycle sends small repeated pulses with a
// mixing delay between them ("rapid on/off" strategy).

static constexpr unsigned long CONTROL_INTERVAL_MS = 10UL * 1000UL;  // 10s between cycles
static constexpr unsigned long MIXING_TIME_MS      = 10UL * 1000UL;  // 10s mixing per pulse

static constexpr float PULSE_ML_PH = 5.0f;            // mL per pH adjustment pulse
static constexpr float PULSE_ML_EC = 1.00f;            // mL per nutrient pulse

static constexpr int MAX_PH_PULSES_PER_CYCLE = 10;
static constexpr int MAX_EC_PULSES_PER_CYCLE = 10;

// -----------------------------
// Debug toggles
// -----------------------------

static constexpr bool PRINT_SENSOR_READINGS = true;
static constexpr bool PRINT_DOSING_ACTIONS  = true;

#endif
