#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include "../utils/Time.hpp"

#include <functional>
#include <string>
#include <vector>

class Timer {
private:
  std::string name;
  long end_time;
  std::function<void()> callable;
  static std::vector<Timer *> timers;

  Timer(std::string i_name, int delay_ms, std::function<void()> i_func)
      : name(i_name), callable(i_func) {
    end_time = Time::Now() + (delay_ms * 1E3);

    DEBUG_DEBUG("Timer Start: %d    End: %d", Time::Now(), end_time);
  };

  void run() {
    DEBUG_INFO("Timer.run %d", name.c_str());
    callable();
  }

public:
  static void Tick() {
    for (int i = 0; i < timers.size(); ++i) {
      Timer *timer = timers[i];
      if (timer->end_time <= Time::Now()) {
        timer->run();
        timers.erase(timers.begin() + i--);
        delete timer;
      }
    }
  };

  static void Start(std::string name, int delay_ms,
                    std::function<void()> func) {
    timers.push_back(new Timer(name, delay_ms, func));
  }
};
std::vector<Timer *> Timer::timers = std::vector<Timer *>();
#endif