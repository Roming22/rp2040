#include "mymk/Keyboard.hpp"

void setup() {
  Serial.begin(9600);
  // delay(5000);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[TIMER]");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");

  Keyboard::Setup();

  Serial.println("");
  Serial.println("# Looping");
}

void loop() { Keyboard::Tick(); }
