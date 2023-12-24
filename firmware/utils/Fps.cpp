#include "Fps.h"

#include "Debug.hpp"
#include "Time.h"

namespace utils {
void FPS::Tick() {
  int display_every = 30; // seconds
  static unsigned int counter = 0;
  static unsigned long time = Time::Now();
  ++counter;
  if (Time::Now() - time > display_every * 1E6) {
    DEBUG_INFO("FPS: %d", (int)((1E6 * counter) / (Time::Now() - time)));
    time = micros();
    counter = 0;
  }
}
} // namespace utils