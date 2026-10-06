#include "../firmware/room-climate-hub-live-dashboard/control.h"
#include <assert.h>
#include <limits>
int main() {
  Control c;
  c.command(true); assert(!c.relay);
  c.sample(true, 0, true); c.command(true); assert(c.relay && c.motion);
  c.sample(false, 100, true); assert(c.relay && !c.motion);
  c.sample(true, 600, true); assert(!c.relay && !c.requested && !c.valid);
  c.sample(true, 50, true); assert(!c.relay); // fault recovery never restarts actuator
  c.command(true); c.sample(true, 10, false); assert(!c.relay);
  c.sample(false, std::numeric_limits<float>::quiet_NaN(), true); assert(!c.valid);
  c.sample(false, -21, true); assert(!c.valid);
  c.sample(false, 500, true); assert(c.valid);
  c.command(false); assert(!c.relay);
}
