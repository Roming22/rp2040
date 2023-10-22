#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include <functional>
#include <string>
#include <vector>

class Timer {
private:
  std::string name;
  long end_time;
  std::function<void()> callable;
  static std::vector<Timer *> timers;

  Timer(std::string i_name, int delay_ms, std::function<void()> i_func);

  void run();

public:
  static void Tick();

  static void Start(std::string name, int delay_ms, std::function<void()> func);
};
#endif