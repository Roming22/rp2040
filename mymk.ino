#include <string>

#define DATA_PIN 1
#define PIXELS_PIN 0
#define PIXELS_COUNT 4

bool isLeft = 0;

#include "mymk/mymk.hpp"

#include "loop/loop.hpp"

void setup() {
  Serial.begin(9600);
  delay(5000);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[KEYMATRIX]");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");

  set_board();
  set_pixels(isLeft);
  set_bitbang();

  Serial.println("");
  Serial.println("# Looping");
}

void loop() {
  // loop_1_relay();
  // loop_2_echo();
  // loop_3_sync();
  // loop_4_data();
  loop_5_keymatrix();
}
