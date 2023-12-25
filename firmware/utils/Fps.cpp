#include "Fps.h"

#include "Debug.hpp"
#include "Time.h"

#include <map>

namespace utils {
void FPS::Tick(const std::string &name) {
  int display_every = 30; // seconds
  static std::map<std::string, int> counter;
  static std::map<std::string, unsigned long> time;
  unsigned long then = Time::Now();
  auto fps = counter.find(name);
  int value = 0;
  if (fps != counter.end()) {
    value = fps->second;
    then = time[name];
  } else {
    time[name] = then;
  }
  counter[name] = value + 1;
  if (Time::Now() - then > display_every * 1E6) {
    DEBUG_INFO("FPS %s: %d", name.c_str(),
               (int)((1E6 * value) / (Time::Now() - then)));
    time[name] = Time::Now();
    counter[name] = 0;
  }
}
} // namespace utils