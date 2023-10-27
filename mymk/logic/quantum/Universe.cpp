#include "Universe.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Memory.h"
#include "../Events.h"
#include "Timeline.h"

#include <string>

void Universe::Setup(std::string layer_name) {
  DEBUG_INFO("Universe::Setup %s", layer_name.c_str());
  // Activates the default layer.
  DEBUG_INFO("Universe: Loading %s", layer_name.c_str());
  start_timeline = new Timeline(layer_name, nullptr);
  std::string switch_id("switch.1");
  Layer::Get(layer_name).on_press(*start_timeline, switch_id, true);
  start_timeline = start_timeline->resolve();
  std::string release_event = switch_id + ".released";
  start_timeline->process_event(release_event);
  Memory::printFreeMemory();
}

void Universe::Tick() {
  DEBUG_VERBOSE("Universe::Tick");
  if (Event::HasEvents()) {
    std::string event_id = Event::Get();
    DEBUG_DEBUG("Universe: processing the '%s' event", event_id.c_str());
    start_timeline->process_event(event_id);
    start_timeline = start_timeline->resolve();
    Memory::printFreeMemory();
  }
}

Timeline *Universe::start_timeline = nullptr;
