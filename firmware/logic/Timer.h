#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include <functional>
#include <map>
#include <string>

namespace logic {
class Timer {
private:
  long end_time;
  std::function<void()> callable;
  static std::map<std::string, Timer *> timers;

  Timer(const std::string &i_name, const int &delay_ms);

  void add_event();
  bool tick();

public:
  std::string name;
  static void Tick();

  static Timer *Start(const std::string &name, const int &delay_ms);

  static void Stop(const std::string &name);
};
} // namespace logic
#endif