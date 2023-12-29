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

  // Receive GO
  if (isLeft) {
    Serial.println("GET");
    // if (BitBang::receiveSync(25000)) {
    //   Serial.println("Synced");
    msg = BitBang::receive(msgLen);
    // }
  } else {
    // Send GO
    delay(1); // Communication currently unstable without an added delay
    Serial.println("");
    Serial.println("POST");
    // if (BitBang::sendSync(25000)) {
    //   Serial.println("Synced");
    BitBang::send(loopIndex, msgLen);
    // }
  }

  if (isLeft && loopIndex != msg) {
    if (loopIndex > 10) {
      Serial.println("Value: ");
      Serial.println(msg);
      Serial.println("");
      Serial.println("");
      Serial.print("!!! Bad value: ");
      Serial.print(++badValues);
      Serial.println(" !!!");
    }
    // delay(2000);
    loopIndex = msg;
  }
  Serial.println("");
  Serial.println("BLINK");
  if (loopIndex % 100 == 0 && badValues) {
    Serial.print("Bad value check: ");
    Serial.print(badValues);
    pixels.fill(pixels.Color(255 * (badValues > 0), 255 * (badValues == 0), 0));
    pixels.show();
    delay(2000);
  }
  blinkLeds();
}
