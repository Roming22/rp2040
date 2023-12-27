#ifndef MYMK_LOGIC_TIMER
#define MYMK_LOGIC_TIMER

#include <functional>
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
  static unsigned long now;
  typedef std::shared_ptr<Timer> Ptr;
  const std::string name;

private:
  long end_time;
  quantum::Timeline *timeline;
  static std::function<void(Timer &)> callback;
  static std::list<Timer *> timers;

  Timer(const std::string &i_name, const int &delay_ms,
        quantum::Timeline *timeline);

  bool tick();

public:
  ~Timer();
  static void Tick();

  static Timer::Ptr Start(const std::string &name, const int &delay_ms,
                          quantum::Timeline *timeline);

  void unregister();
};
} // namespace logic
#endif