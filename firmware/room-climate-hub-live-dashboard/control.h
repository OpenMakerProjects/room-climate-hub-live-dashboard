#pragma once
#include <stdint.h>
#include <math.h>
struct Control {
  bool requested = false;
  bool relay = false;
  bool valid = false;
  bool motion = false;
  float current_mA = 0;
  void sample(bool sensedMotion, float current, bool sensorOK) {
    motion = sensedMotion;
    current_mA = current;
    valid = sensorOK && isfinite(current) && current >= -20 && current <= 500;
    relay = requested && valid;
    if (!valid) requested = false;
  }
  void command(bool on) { requested = on && valid; relay = requested; }
};
