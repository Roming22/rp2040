#include "Universe.h"
#include "../Events.h"
#include "Timeline.h"

#include "../../utils/Debug.hpp"

void Universe::Setup(std::string layer_name) {
  DEBUG_INFO("Universe::Setup %s", layer_name.c_str());
  start_timeline = new Timeline("start", nullptr);

  // The 'start' event activates the default layer.
  start_timeline->possible_events["start"] = "LY_TO(" + layer_name + ")";
  Event::Add(std::string("start"));
}

void Universe::Tick() {
  DEBUG_VERBOSE("Universe::Tick");
  if (Event::HasEvents()) {
    std::string event_id = Event::Get();
    DEBUG_INFO("Universe: processing the '%s' event", event_id.c_str());
    start_timeline->process_event(event_id);
    start_timeline = start_timeline->resolve();
  }
}

Timeline *Universe::start_timeline = nullptr;
