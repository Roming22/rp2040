#include "Timer.h"

#include "../logic/Events.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"
#include "ObjectManager.h"
#include "quantum/Universe.h"
#include "typedef.h"
#include <functional>

namespace logic {
std::function<void(Timer &)> Timer::callback = [](Timer &timer) {
  logic::Event::Add(timer.name, timer.end_time, timer.timeline);
};

Timer::Timer(const std::string &i_name, const int &delay_ms,
             quantum::Timeline *i_timeline)
    : name(i_name), timeline(i_timeline) {
  // TODO sort timers by end_time
  // This will allow to exit the Tick loop early, as well
  // as ensure the ordering of the events.
  timers.push_back(this);
  end_time = utils::Time::Now() + (delay_ms * 1E3);
  logic::ObjectManager::Register("logic::Timer");
};

Timer::~Timer() {
  // DEBUG_INFO("[DELETE %d] logic::Timer (%d)", this, timers.size());
  unregister();
  logic::ObjectManager::Unregister("logic::Timer");
}

void Timer::Tick() {
  std::vector<Timer *> triggered;
  for (auto timer : timers) {
    if (timer->tick()) {
      triggered.push_back(timer);
    }
  }
  for (auto timer : triggered) {
    timer->unregister();
  }
}

Timer::Ptr Timer::Start(const std::string &name, const int &delay_ms,
                        quantum::Timeline *timeline) {
  DEBUG_INFO("logic::Timer::Start %s", name.c_str());
  return Ptr(new Timer(name, delay_ms, timeline));
}

void Timer::unregister() {
  auto item = std::find(timers.begin(), timers.end(), this);
  if (item != timers.end()) {
    timers.erase(item);
  }
}

bool Timer::tick() {
  // DEBUG_VERBOSE("logic::Timer::tick %s", name.c_str());
  if (end_time <= utils::Time::Now()) {
    // Having a call to Event::Add here tanked the performance
    // of the Keyboard::Tick loop (in multicore mod), by almost
    // 2/3 (from 28kHz to 11kHz). Using the callback seems to
    // trick the compiler to better optimize the loop.
    callback(*this);
    return true;
  }
  return false;
}

std::list<Timer *> Timer::timers = std::list<Timer *>();
} // namespace logic