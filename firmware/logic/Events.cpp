#include "Events.h"
#include "../utils/Debug.hpp"

namespace logic {
void Event::Add(const std::string &id) {
  DEBUG_DEBUG("logic::Event::Add %s", id.c_str());
  ids.push(id);
}

bool Event::HasEvents() {
  DEBUG_VERBOSE("logic::Event::HasEvents");
  return !ids.empty();
}

std::string Event::Get() {
  DEBUG_VERBOSE("logic::Event::Get");
  std::string event_id = ids.front();
  ids.pop();
  return event_id;
}

std::queue<std::string> Event::ids = std::queue<std::string>();
} // namespace logic