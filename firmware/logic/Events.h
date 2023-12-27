#ifndef MYMK_LOGIC_EVENTS
#define MYMK_LOGIC_EVENTS

#include "quantum/Timeline.h"

#include "Timer.h"
#include <memory>
#include <queue>

namespace logic {
class Event {
public:
  typedef std::shared_ptr<Event> Ptr;

  const std::string id;
  const unsigned long time;
  quantum::Timeline *timeline;

private:
  static std::list<Ptr> events;

  Event(const std::string &id, const unsigned long time,
        quantum::Timeline *timeline);

public:
  ~Event();

  static void Add(const std::string &id, const unsigned long time,
                  quantum::Timeline *timeline);
  static bool HasEvents();
  static Event &Peek();
  static Ptr Get();
};
} // namespace logic
#endif