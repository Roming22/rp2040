#include "Timer.h"
#include "../logic/Events.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"

namespace logic {
Timer::Timer(const std::string &i_name, const int &delay_ms) : name(i_name) {
  end_time = utils::Time::Now() + (delay_ms * 1E3);

  DEBUG_INFO("Timer %s @%d    Start: %d    End: %d", name.c_str(), this,
             utils::Time::Now(), end_time);
};

void Timer::Tick() {
  std::vector<std::string> to_delete;
  for (auto pair : timers) {
    if (pair.second->tick()) {
      to_delete.push_back(pair.second->name);
    }
  }
  for (const std::string name : to_delete) {
    Stop(name);
  }
}

Timer *Timer::Start(const std::string &name, const int &delay_ms) {
  if (timers.count(name) > 0) {
    Stop(name);
  }
  timers[name] = new Timer(name, delay_ms);
  return timers[name];
}

void Timer::Stop(const std::string &name) {
  DEBUG_INFO("logic::Timer::Stop %s", name.c_str());
  if (timers.count(name) > 0) {
    Timer *timer = timers[name];
    timers.erase(name);
    delete timer;
  } else {
    DEBUG_WARNING("Timer not found in map: %s", name.c_str());
  }
}

void Timer::add_event() {
  DEBUG_VERBOSE("logic::Timer::add_event %s @%d", name.c_str(), this);
  Event::Add(name);
}

bool Timer::tick() {
  DEBUG_VERBOSE("logic::Timer::tick %s", name.c_str());
  if (end_time <= utils::Time::Now()) {
    add_event();
    return true;
  }
  return false;
}

std::map<std::string, Timer *> Timer::timers = std::map<std::string, Timer *>();
} // namespace logic