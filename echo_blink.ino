#define DATA_PIN 1
// #define DATA_FREQ 8192
// #define DATA_FREQ 32768
#define DATA_FREQ 50000
// #define DATA_FREQ 125000
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

  // Receive GO
  if (isLeft) {
    Serial.println("GET");
    // msg = BitBang::receive(msgLen);
    msg = BitBang::receive(msgLen);
  } else {
    // Send GO
    Serial.println("POST");
    BitBang::send(loopIndex, msgLen);
  }

  if (isLeft && loopIndex != msg) {
    if (msg > 10) {
      Serial.println("Value: ");
      Serial.println(msg);
      Serial.println("");
      Serial.println("");
      Serial.print("!!! Bad value: ");
      Serial.print(++badValues);
      Serial.println(" !!!");
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
    Serial.print(badValues);
    pixels.fill(pixels.Color(255 * (badValues > 0), 255 * (badValues == 0), 0));
    pixels.show();
    delay(2000);
  }
  blinkLeds((loopIndex - 1) % 6);
  if (loopIndex % 20000 == 0) {
    delay(3000);
  }
}
