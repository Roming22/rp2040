#ifndef MYMK_LOGIC_EVENTS
#define MYMK_LOGIC_EVENTS

#include <queue>
#include <string>

namespace logic {
class Event {
private:
  static std::queue<std::string> ids;

public:
  static void Add(const std::string &id);

  static bool HasEvents();

  static std::string Get();
};
} // namespace logic
#endif