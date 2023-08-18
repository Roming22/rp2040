#include <Adafruit_NeoPixel.h>

#include "mymk/mymk.hpp"

#define DATA_PIN     1
#define PIXELS_PIN   0
#define PIXELS_COUNT 4

#define DELAY   25

int loopIndex = 0;
bool isLeft = 0;


void setup() {
  Serial.begin(9600);
  delay(1300);

  for(int i=10; i>0; --i) {
    Serial.println();
  }
  
  Serial.println("");
  Serial.println("################################################################################");
  Serial.println("# Rebooting");

  int seed = micros();
  Serial.print("Random seed: ");
  Serial.println(seed);
  randomSeed(seed);

  Pixels::initialize(PIXELS_PIN, PIXELS_COUNT);
  BitBang::initialize(DATA_PIN, 1 << 13);
  Pixels& pixels = *Pixels::get();

  isLeft = micros() % 2;
  if (isLeft == 1) {
    Serial.println("Left");
    pixels.fill(pixels.Color(255, 0, 0));
  } else {
    Serial.println("Right");
    pixels.fill(pixels.Color(0, 255, 0));
  }
  pixels.show();
  delay(2000);
  pixels.fill(pixels.Color(0, 0 ,0));
  pixels.show();

  Serial.println("");
  Serial.println("# Looping");
}

void blinkLeds(const int count) {
  Pixels& pixels = *Pixels::get();
  for(int i=count; i>0; --i) {
    if (isLeft) {
      pixels.fill(pixels.Color(255, 0, 0));
    } else {
      pixels.fill(pixels.Color(0, 255, 0));
    }
    pixels.show();
    delay(DELAY);
    pixels.fill(pixels.Color(0, 0 ,0));
    pixels.show();
    delay(DELAY);
  }
}

void loop() {
  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
  } else {
    Serial.println(" [Extension]");
  }
  int value = 0;

  // Wait for go
  if (isLeft !=  loopIndex) {
    while (BitBang::receive() == 0);
  }

  blinkLeds(1);

  // Send go
  BitBang::send();
}
