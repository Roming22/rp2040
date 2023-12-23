#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include <map>
#include <memory>
#include <string>

namespace logic {

// Forward declaration, probably some code smell
namespace quantum {
class Timeline;
}

class Timer {
public:
  typedef std::shared_ptr<Timer> Ptr;

private:
  bool disabled;
  long end_time;
  quantum::Timeline *timeline;
  static std::map<quantum::Timeline *, Timer::Ptr> timers;

  Timer(const std::string &i_name, const int &delay_ms,
        quantum::Timeline *timeline);

  void send_event();
  bool tick();

public:
  std::string name;
  ~Timer() { DEBUG_INFO("[DELETE %d] logic::Timer", this); }
  static void Tick();

  static Timer::Ptr Start(const std::string &name, const int &delay_ms,
                          quantum::Timeline *timeline);

  void stop();
};
} // namespace logic
#endif