#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include <list>
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
  const std::string name;

private:
  bool active;
  long end_time;
  quantum::Timeline *timeline;
  static std::list<Timer *> timers;

  Timer(const std::string &i_name, const int &delay_ms,
        quantum::Timeline *timeline);

  void send_event();
  bool tick();

public:
  ~Timer();
  static void Tick();

  static Timer::Ptr Start(const std::string &name, const int &delay_ms,
                          quantum::Timeline *timeline);

  void stop();
  void unregister();
};
} // namespace logic
#endif