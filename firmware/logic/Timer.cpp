#include "Timer.h"
#include "../logic/Events.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"
#include "quantum/Universe.h"
#include "typedef.h"

namespace logic {
Timer::Timer(const std::string &i_name, const int &delay_ms,
             quantum::Timeline *i_timeline)
    : name(i_name), timeline(i_timeline), disabled(false) {
  end_time = utils::Time::Now() + (delay_ms * 1E3);

  DEBUG_INFO("[CREATE %d] logic::Timer %s    Start: %d    End: %d", this,
             name.c_str(), utils::Time::Now(), end_time);
};

void Timer::Tick() {
  std::vector<quantum::Timeline *> to_delete;
  for (auto pair : timers) {
    if (pair.second->tick()) {
      to_delete.push_back(pair.first);
    }
  }
  if (!to_delete.empty()) {
    logic::quantum::Universe::Resolve();
  }
  for (auto item : to_delete) {
    timers.erase(item);
  }
}

Timer::Ptr Timer::Start(const std::string &name, const int &delay_ms,
                        quantum::Timeline *timeline) {
  DEBUG_INFO("logic::Timer::Start %s", name.c_str());
  timers[timeline] = Timer::Ptr(new Timer(name, delay_ms, timeline));
  DEBUG_INFO("Adding timer %d to map", timeline);
  return timers[timeline];
}

void Timer::stop() {
  DEBUG_INFO("logic::Timer::stop %s", timeline->name.c_str());
  disabled = true;
}

void Timer::send_event() {
  DEBUG_INFO("");
  DEBUG_INFO("############################################################");
  DEBUG_INFO("# @%dms Event: %s ", utils::Time::Now(), name.c_str());
  DEBUG_INFO("############################################################");
  utils::Memory::PrintMemoryUsage();
  timeline->process_event(name);
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

std::map<quantum::Timeline *, Timer::Ptr> Timer::timers =
    std::map<quantum::Timeline *, Timer::Ptr>();
} // namespace logic