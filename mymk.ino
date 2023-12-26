#define MULTICORE_ENABLED 1
#define DEBUG_ENABLED 1
#define FPS_DELAY 2

// #define SWITCH_CORES

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

  Serial.println("");
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
#ifdef FPS_DELAY
  utils::FPS::delay = FPS_DELAY;
#endif
}

int flash;

#define core0 loop
void core0() {
  int old = 0;
  // This function will automatically loop, but
  // adding the loop in the function has a massive
  // impact on the performance (from 14kHz to 25kHz).
  while (true) {
#ifndef SWITCH_CORES
    utils::Time::Tick();
    utils::FPS::Tick("Core0 Universe");
    // logic::quantum::Universe::Tick();
    delay(10);
#else
    utils::FPS::Tick("Core0 Keyboard");
    firmware::Keyboard::Tick();
#endif
    if (flash != old) {
      hardware::led::Pixels::Set(4, 255 * flash, 0, 255 * flash);
      old = flash;
    }
  }
}

void core1() {
  delay(10);
  int old = 0;
  while (true) {
    flash = int(millis() / 1000) % 2;
    if (flash != old) {
      hardware::led::Pixels::Set(2, 0, 255 * flash, 255 * flash);
      Serial.println("He11o");
      old = flash;
    }
#ifndef SWITCH_CORES
    //     utils::FPS::Tick("Core1 Keyboard");
    //     firmware::Keyboard::Tick();
    delay(10);
#else
    delay(10);
// utils::FPS::Tick("Core1 Universe");
// logic::quantum::Universe::Tick();
#endif
  }
}
