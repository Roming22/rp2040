#include <Adafruit_NeoPixel.h>
#include "mymk/mymk.hpp"

#define PIXELS_PIN   0
#define PIXELS_COUNT 4

int loopIndex = 0;
int isLeft = 0;


void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("################################################################################");
  Serial.println("# Rebooting");

  // Seed the pseudo random number generator by using noise on pins
  int seed = micros() % 90 + 10 ;
  Serial.print("Random seed: ");
  Serial.println(seed);
  randomSeed(seed);

  isLeft = random(2);
  if (isLeft == 1) {
    Serial.println("Left");
  } else {
    Serial.println("Right");
  }

  BitBang::initialize(0, 4);

  Pixels::initialize(PIXELS_PIN, PIXELS_COUNT);

  Serial.println("# Looping");
}

void loop() {
  Serial.print("## Loop ");
  Serial.println(++loopIndex);

  Serial.print("[");
  if (isLeft == 1) {
    Serial.print("Left");
  } else {
    Serial.print("Right");
  }
  Serial.println("] Hello World!!!");

  auto pixels = Pixels::get();
  pixels->fill(pixels->Color(0, 0 ,0));
  pixels->show();
  delay(300);
  pixels->fill(pixels->Color(255*isLeft, 255*(1-isLeft) ,0));
  pixels->show();
  delay(700);
}
