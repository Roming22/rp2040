#include "Arduino_DebugUtils.h"

#include "mymk/Keyboard.hpp"

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
  delay(1000);

  DEBUG_INFO("\n\n\n\n\n\n\n\n\n\n");
  DEBUG_INFO("[LAYER]");
  DEBUG_INFO("#############################################################"
             "###################");
  DEBUG_INFO("# Rebooting");

  Keyboard::Setup();

  DEBUG_INFO("");
  DEBUG_INFO("# Looping");
}

void loop() { Keyboard::Tick(); }
