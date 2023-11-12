#include "mymk.hpp"

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
  delay(1000);

  DEBUG_INFO("\n\n\n\n\n\n\n\n\n\n");
  DEBUG_INFO("[LAYER]");
  DEBUG_INFO("#############################################################"
             "###################");
  DEBUG_INFO("# Rebooting");

  // Debug.setDebugLevel(DBG_VERBOSE);
  Firmware::Setup();

  DEBUG_INFO("");
  DEBUG_INFO("# Looping");
}

void loop() { Firmware::Tick(); }
