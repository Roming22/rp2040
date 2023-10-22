#ifndef MYMK_UTILS_TIME
#define MYMK_UTILS_TIME

#include <Arduino.h>

class Time {
  static long now;

public:
  inline static void Tick() { now = micros(); }

  inline static long Now() { return now; }
};
#endif