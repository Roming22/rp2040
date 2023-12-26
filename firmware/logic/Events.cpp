#include "Events.h"

#include "../utils/Debug.hpp"
#include "ObjectManager.h"

namespace logic {
Event::Event(const std::string &i_id, const unsigned long i_time,
             quantum::Timeline *i_timeline)
    : id(i_id), time(i_time), timeline(i_timeline) {
  ObjectManager::Register("logic::Event");
}

Event::~Event() { ObjectManager::Unregister("logic::Event"); }

void Event::Add(const std::string &id, const unsigned long time,
                quantum::Timeline *timeline) {
  DEBUG_VERBOSE("logic::Event::Add %s at %dms", id.c_str(), time);
  // The oldest event is at the end of the list.
  auto index = events.begin();
  for (; index != events.end(); ++index) {
    if (time >= (*index)->time) {
      break;
    }
  }
  events.insert(index, Ptr(new Event(id, time, timeline)));
}

bool Event::HasEvents() {
  DEBUG_VERBOSE("logic::Event::HasEvents");
  return !events.empty();
}

Event::Ptr Event::Get() {
  DEBUG_INFO("logic::Event::Get");

  for (auto event : events) {
    DEBUG_INFO("- %dus: %s", event->time, event->id.c_str());
  }

  Ptr event = events.back();
  events.pop_back();
  return event;
}

std::list<Event::Ptr> Event::events = std::list<Event::Ptr>();
} // namespace logic