#ifndef MYMK_UTILS_TIME
#define MYMK_UTILS_TIME
class Time {
  static long now;

public:
  static void Tick() { now = micros(); }

  static long Now() { return now; }
};
long Time::now = 0;
#endif