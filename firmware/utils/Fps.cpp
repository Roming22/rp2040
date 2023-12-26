#include "Fps.h"

#include "Debug.hpp"
#include "Memory.h"
#include "Time.h"

#include <map>

namespace utils {
int FPS::delay = 0; // display every seconds
void FPS::Tick(const std::string &name) {
  if (delay == 0) {
    return;
  }
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
  if (Time::Now() - then > delay * 1E6) {
    // for (auto kvp : time) {
    // std::string _name = kvp.first;
    // then = kvp.second;
    // value = counter[_name];
    DEBUG_INFO("FPS %s: %d (Memory: %.2f%%)", name.c_str(),
               (int)((1E6 * value) / (Time::Now() - then)),
               Memory::GetPctUsed());
    time[name] = Time::Now();
    counter[name] = 0;
  }
}
} // namespace utils