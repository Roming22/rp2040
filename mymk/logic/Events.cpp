#include "Events.h"
#include "../utils/Debug.hpp"

#include <queue>
#include <string>

void Event::Add(const std::string &id) {
  DEBUG_INFO("Event added: %s", id.c_str());
  ids.push(id);
}

bool Event::HasEvents() { return !ids.empty(); }

std::string Event::Get() {
  std::string event_id = ids.front();
  ids.pop();
  return event_id;
}

std::queue<std::string> Event::ids = std::queue<std::string>();
