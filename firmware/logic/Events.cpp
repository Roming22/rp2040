#include "Events.h"
#include "../utils/Debug.hpp"

#include <queue>
#include <string>

namespace logic {
void Event::Add(const std::string &id) {
  DEBUG_INFO("Event::Add %s", id.c_str());
  ids.push(id);
}

bool Event::HasEvents() {
  DEBUG_VERBOSE("Event::HasEvents");
  return !ids.empty();
}

std::string Event::Get() {
  DEBUG_VERBOSE("Event::Get");
  DEBUG_INFO("");
  std::string event_id = ids.front();
  ids.pop();
  return event_id;
}

std::queue<std::string> Event::ids = std::queue<std::string>();
} // namespace logic