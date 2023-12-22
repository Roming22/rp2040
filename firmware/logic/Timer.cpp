#include "Timer.h"
#include "../logic/Events.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"
#include "quantum/Universe.h"

namespace logic {
Timer::Timer(const std::string &i_name, const int &delay_ms,
             quantum::Timeline &i_timeline)
    : name(i_name), timeline(i_timeline), disabled(false) {
  end_time = utils::Time::Now() + (delay_ms * 1E3);

  DEBUG_INFO("Timer %s @%d    Start: %d    End: %d", name.c_str(), this,
             utils::Time::Now(), end_time);
};

void Timer::Tick() {
  std::vector<std::string> to_delete;
  for (auto pair : timers) {
    if (pair.second->tick()) {
      to_delete.push_back(pair.second->name);
      delete pair.second;
    }
  }
  for (const std::string name : to_delete) {
    timers.erase(name);
  }
}

Timer *Timer::Start(const std::string &name, const int &delay_ms,
                    quantum::Timeline &timeline) {
  DEBUG_INFO("logic::Timer::Start %s", name.c_str());
  timers[name] = new Timer(name, delay_ms, timeline);
  return timers[name];
}

void Timer::Stop(const std::string &name) {
  DEBUG_INFO("logic::Timer::Stop %s", name.c_str());
  if (timers.count(name) > 0) {
    Timer &timer = *timers[name];
    timer.disabled = true;
  } else {
    DEBUG_WARNING("Timer not found in map: %s", name.c_str());
  }
}

void Timer::send_event() {
  DEBUG_INFO("");
  DEBUG_INFO("############################################################");
  DEBUG_INFO("# Event: %s @ %d", name.c_str(), utils::Time::Now());
  DEBUG_INFO("############################################################");
  timeline.process_event(name);
  logic::quantum::Universe::Resolve();
}

bool Timer::tick() {
  DEBUG_VERBOSE("logic::Timer::tick %s", name.c_str());
  if (disabled) {
    return true;
  }
  if (end_time <= utils::Time::Now()) {
    send_event();
    return true;
  }
  return false;
}

std::map<std::string, Timer *> Timer::timers = std::map<std::string, Timer *>();
} // namespace logic