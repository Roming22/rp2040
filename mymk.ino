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
  delay(100);
  utils::Time::Tick();
#endif
}

#define core0 loop
void core0() {
#ifdef MULTICORE
  logic::quantum::Universe::Tick();
#else
  firmware::Keyboard::Tick();
#endif
}

void core1() {
  while (true) {
    firmware::Keyboard::Tick();
  }
}