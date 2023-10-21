#ifndef MYMK_LOGIC_EVENTS
#define MYMK_LOGIC_EVENTS

#include <queue>
#include <string>

class Event {
private:
  static std::queue<std::string> ids;

public:
  static void Add(const std::string &id) {
    DEBUG_INFO("Event added: %s", id.c_str());
    ids.push(id);
  }

  static bool HasEvents() { return !ids.empty(); }

  static std::string Get() {
    std::string event_id = ids.front();
    ids.pop();
    return event_id;
  }
};
std::queue<std::string> Event::ids = std::queue<std::string>();
#endif