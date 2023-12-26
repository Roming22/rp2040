#define MULTICORE 1

#include "firmware.hpp"

void setup() {
  Serial.begin(115200);
  Debug.timestampOn();
  while (!Serial) {
    delay(50);
  }

  Debug.timestampOn();
  // Debug.setDebugLevel(DBG_NONE);
  // Debug.setDebugLevel(DBG_ERROR);
  // Debug.setDebugLevel(DBG_WARNING);
  Debug.setDebugLevel(DBG_INFO);
  // Debug.setDebugLevel(DBG_DEBUG);
  // Debug.setDebugLevel(DBG_VERBOSE);

  DEBUG_INFO("\n\n\n\n\n\n\n\n\n\n");
  DEBUG_INFO("#############################################################"
             "###################");
  DEBUG_INFO("# Rebooting");
  DEBUG_INFO("#############################################################"
             "###################");

  firmware::Keyboard::Setup();

  DEBUG_INFO("");
  DEBUG_INFO("#############################################################"
             "###################");
  DEBUG_INFO("# Looping");
  DEBUG_INFO("#############################################################"
             "###################");
#ifdef MULTICORE
  multicore_launch_core1(core1);
#endif
}

#define core0 loop
void core0() { firmware::Keyboard::Tick(); }

void core1() {
  bool flip = true;
  while (true) {
    // logic::quantum::Universe::Tick();
    hardware::led::Pixels::Set(2, 255 * flip, 255 * flip, 255 * flip);
    hardware::led::Pixels::Set(3, 255 * flip, 255 * flip, 255 * flip);
    hardware::led::Pixels::Set(4, 255 * flip, 255 * flip, 255 * flip);
    flip = !flip;
    delay(250);
  }
}
