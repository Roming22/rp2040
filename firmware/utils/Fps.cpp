#include "Fps.h"

#include "Debug.hpp"
#include "Memory.h"

#include <map>

namespace utils {
int FPS::delay = 0; // display every seconds
void FPS::Tick(const std::string &name) {
  if (delay == 0) {
    return;
  }
  static std::map<std::string, int> counter;
  static std::map<std::string, unsigned long> time;
  unsigned long now = Time::Now();
  unsigned long then;
  auto fps = counter.find(name);
  int value = 0;
  if (fps != counter.end()) {
    value = fps->second;
    then = time[name];
  } else {
    time[name] = now;
    then = now;
  }
  counter[name] = value + 1;
  if (now - then > delay * 1E6) {
    // for (auto kvp : time) {
    // std::string _name = kvp.first;
    // then = kvp.second;
    // value = counter[_name];
    DEBUG_INFO("FPS %s: %d (Memory: %.2f%%)", name.c_str(),
               (int)((1E6 * value) / (now - then)), Memory::GetPctUsed());
    time[name] = now;
    counter[name] = 0;
  }
}

void FPS::TimeIt(std::function<void()> function) {
  unsigned long begin = micros();
  function();
  unsigned long end = micros();
  DEBUG_INFO("TimeIt: %dHz", 1E3 / int(end - begin));
}
} // namespace utils