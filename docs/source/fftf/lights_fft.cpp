#include "lights_fft.h"
#include "config_fft.h"

// millis()-based on/off cycle. Timer starts at light_setup()
// (after boot/WiFi) so connection time doesn't offset the schedule.

static unsigned long start_time = 0;
static bool lights_on = false;

void light_setup() {
  pinMode(LIGHT_1_PIN, OUTPUT);
  pinMode(LIGHT_2_PIN, OUTPUT);
  pinMode(LIGHT_3_PIN, OUTPUT);

  start_time = millis();

  digitalWrite(LIGHT_1_PIN, HIGH);
  digitalWrite(LIGHT_2_PIN, HIGH);
  digitalWrite(LIGHT_3_PIN, HIGH);
  lights_on = true;

  Serial.println("Lights ON");
}

void lights_tick() {
  unsigned long elapsed = millis() - start_time;
  unsigned long cycle_ms = LIGHT_ON_DURATION_MS + LIGHT_OFF_DURATION_MS;
  unsigned long pos = elapsed % cycle_ms;
  bool should_be_on = (pos < LIGHT_ON_DURATION_MS);

  if (should_be_on && !lights_on) {
    digitalWrite(LIGHT_1_PIN, HIGH);
    digitalWrite(LIGHT_2_PIN, HIGH);
    digitalWrite(LIGHT_3_PIN, HIGH);
    lights_on = true;
    Serial.println("Lights -> ON");
  } else if (!should_be_on && lights_on) {
    digitalWrite(LIGHT_1_PIN, LOW);
    digitalWrite(LIGHT_2_PIN, LOW);
    digitalWrite(LIGHT_3_PIN, LOW);
    lights_on = false;
    Serial.println("Lights -> OFF");
  }
}
