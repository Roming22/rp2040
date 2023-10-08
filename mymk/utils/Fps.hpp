#ifndef MYMK_UTILS_FPS
#define MYMK_UTILS_FPS

#include "Time.hpp"

class FPS {
public:
  static void Tick() {
    int display_every = 15; // seconds
    static unsigned int counter = 0;
    static unsigned long time = Time::Now();
    ++counter;
    if (Time::Now() - time > display_every * 1E6) {
      Serial.print("FPS: ");
      Serial.println((int)((1E6 * counter) / (Time::Now() - time)));
      time = micros();
      counter = 0;
    }
  }
};
#endif