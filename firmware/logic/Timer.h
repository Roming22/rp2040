#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include "quantum/Timeline.h"

#include <map>
#include <string>

namespace logic {
class Timer {
private:
  bool disabled;
  long end_time;
  quantum::Timeline &timeline;
  static std::map<std::string, Timer *> timers;

  Timer(const std::string &i_name, const int &delay_ms,
        quantum::Timeline &timeline);

  void send_event();
  bool tick();

public:
  std::string name;
  static void Tick();

  static Timer *Start(const std::string &name, const int &delay_ms,
                      quantum::Timeline &timeline);

  static void Stop(const std::string &name);
};
} // namespace logic
#endif