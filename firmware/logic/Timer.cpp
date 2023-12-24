#include "Timer.h"
#include "../logic/Events.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"
#include "quantum/Universe.h"
#include "typedef.h"

namespace logic {
Timer::Timer(const std::string &i_name, const int &delay_ms,
             quantum::Timeline *i_timeline)
    : name(i_name), timeline(i_timeline), active(true) {
  timers.push_back(this);
  end_time = utils::Time::Now() + (delay_ms * 1E3);

  DEBUG_INFO("[CREATE %d] logic::Timer %s    Start: %d    End: %d", this,
             name.c_str(), utils::Time::Now(), end_time);
};

Timer::~Timer() {
  DEBUG_INFO("[DELETE %d] logic::Timer", this);
  unregister();
}

void Timer::Tick() {
  std::vector<Timer *> triggered;
  for (auto timer : timers) {
    if (timer->tick()) {
      triggered.push_back(timer);
    }
  }
  if (!triggered.empty()) {
    logic::quantum::Universe::Resolve();
  }
  for (auto timer : triggered) {
    timer->unregister();
  }
}

Timer::Ptr Timer::Start(const std::string &name, const int &delay_ms,
                        quantum::Timeline *timeline) {
  DEBUG_INFO("logic::Timer::Start %s", name.c_str());
  return Timer::Ptr(new Timer(name, delay_ms, timeline));
}

void Timer::unregister() {
  auto item = std::find(timers.begin(), timers.end(), this);
  if (item != timers.end()) {
    timers.erase(item);
  }
}

void Timer::stop() {
  DEBUG_INFO("logic::Timer::stop %d: %s", this, timeline->name.c_str());
  active = false;
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
  if (active && end_time <= utils::Time::Now()) {
    send_event();
    return true;
  }
  return false;
}

std::list<Timer *> Timer::timers = std::list<Timer *>();
} // namespace logic