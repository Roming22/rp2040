#define MULTICORE_ENABLED 1
#define DEBUG_ENABLED 1

#define SWITCH_CORES

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

  Serial.println("\n\n\n\n\n\n\n\n\n\n");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");
  Serial.println("#############################################################"
                 "###################");

  firmware::Keyboard::Setup();

  Serial.println("");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Looping");
  Serial.println("#############################################################"
                 "###################");
#ifdef MULTICORE_ENABLED
  multicore_launch_core1(core1);
#endif
}

int flash;

#define core0 loop
void core0() {
  static int old = 0;
#ifdef SWITCH_CORES
  firmware::Keyboard::Tick();
#else
  logic::quantum::Universe::Tick();
#endif
  if (flash != old) {
    hardware::led::Pixels::Set(4, 255 * flash, 0, 255 * flash);
    old = flash;
  }
}

void core1() {
  int old = 0;
  while (true) {
    delay(10);
    flash = int(millis() / 1000) % 2;
    if (flash != old) {
      hardware::led::Pixels::Set(2, 0, 255 * flash, 255 * flash);
      old = flash;
    }
#ifdef SWITCH_CORES
    // logic::quantum::Universe::Tick();
#else
    // firmware::Keyboard::Tick();
#endif
  }
}
