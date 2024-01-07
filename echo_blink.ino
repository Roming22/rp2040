#define DATA_PIN 1
// #define DATA_FREQ 8064
#define DATA_FREQ 125000
#define PIXELS_PIN 0
#define PIXELS_COUNT 4
bool isLeft = 0;

#include <string>

#include "mymk/mymk.hpp"

void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 1;
  static unsigned int badValues = 0;
  Pixels &pixels = *Pixels::get();

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
  } else {
    Serial.println(" [Extension]");
  }

  if (isLeft) {
    // Serial.println("GET");
    msg = BitBang::receive(msgLen);
    // delay(1000); // This delay breaks the communication.
  } else {
    // Serial.println("POST");
    BitBang::send(loopIndex, msgLen);
  }

  if (isLeft && loopIndex != msg) {
    if (msg > 10) {
      Serial.print("!!! Bad value #");
      Serial.print(++badValues);
      Serial.print(": ");
      Serial.println(msg);
    } else {
      badValues = 0;
    }
    // delay(2000);
    loopIndex = msg;
  }
  // Serial.println("");
  // Serial.println("BLINK");
  if (loopIndex % 100 == 0 && badValues) {
    Serial.print("Bad value check: ");
    Serial.println(badValues);
    delay(2000);
  }
  blinkLeds((loopIndex - 1) % 6);
}
