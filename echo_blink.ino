#define DATA_PIN 1
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
    msg = BitBang::receive(msgLen);
  } else {
    delay(50);
    // Send GO
    Serial.println("");
    Serial.println("POST");
    BitBang::send(loopIndex, msgLen);
  }

  if (isLeft && loopIndex != msg) {
    Serial.println("");
    Serial.println("");
    Serial.print("!!! Bad value: ");
    Serial.print(++badValues);
    Serial.println(" !!!");
    // delay(2000);
  }
  Serial.println("");
  Serial.println("BLINK");
  if (loopIndex % 500 == 0) {
    Serial.print("Bad value check: ");
    Serial.print(badValues);
    pixels.fill(pixels.Color(255 * (badValues > 0), 255 * (badValues == 0), 0));
    pixels.show();
    delay(2000);
  }
  blinkLeds();
}
