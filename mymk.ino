#include "firmware.hpp"

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
  while (!Serial) {
    delay(50);
  }

  DEBUG_INFO("\n\n\n\n\n\n\n\n\n\n");
  DEBUG_INFO("[LAYER]");
  DEBUG_INFO("#############################################################"
             "###################");
  DEBUG_INFO("# Rebooting");

  // Debug.setDebugLevel(DBG_VERBOSE);
  firmware::Keyboard::Setup();

  DEBUG_INFO("");
  DEBUG_INFO("# Looping");
}

void loop() { firmware::Keyboard::Tick(); }
