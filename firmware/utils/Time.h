#ifndef MYMK_UTILS_TIME
#define MYMK_UTILS_TIME

#include <Arduino.h>

namespace utils {
class Time {
  static long now;

public:
  inline static void Tick() { now = micros(); }

  inline static long Now() { return now; }
};
} // namespace utils
#endif