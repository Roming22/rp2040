#include "Timer.h"
#include "../utils/Debug.hpp"
#include "../utils/Time.h"

Timer::Timer(std::string i_name, int delay_ms, std::function<void()> i_func)
    : name(i_name), callable(i_func) {
  end_time = Time::Now() + (delay_ms * 1E3);

  DEBUG_DEBUG("Timer Start: %d    End: %d", Time::Now(), end_time);
};

void Timer::run() {
  if (name[0] != '#') {
    DEBUG_INFO("Timer.run %s", name.c_str());
  }
  callable();
}

void Timer::Tick() {
  for (int i = 0; i < timers.size(); ++i) {
    Timer *timer = timers[i];
    if (timer->end_time <= Time::Now()) {
      timer->run();
      timers.erase(timers.begin() + i--);
      delete timer;
    }
  }
}

void Timer::Start(std::string name, int delay_ms, std::function<void()> func) {
  timers.push_back(new Timer(name, delay_ms, func));
}

std::vector<Timer *> Timer::timers = std::vector<Timer *>();
