#include "Universe.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Memory.h"
#include "../Events.h"
#include "../feature/Layer.h"
#include "Timeline.h"

#include <string>

namespace logic {
namespace quantum {
void Universe::Setup(std::string layer_name) {
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Universe::Setup %s", layer_name.c_str());
  // Activates the default layer.
  DEBUG_INFO("Universe: Loading %s", layer_name.c_str());
  start_timeline = new Timeline(layer_name, nullptr, 1);
  std::string switch_id("switch.1");
  logic::feature::Layer::OnPress(layer_name, *start_timeline, switch_id, true);
  start_timeline->resolve();
  std::string release_event = switch_id + ".released";
  start_timeline->process_event(release_event);
}

void Universe::StartTimeline(Timeline &timeline) {
  DEBUG_INFO("logic::quantum::Universe::StartTimeline %s",
             timeline.history.c_str());
  start_timeline = &timeline;
}

void Universe::Resolve() { start_timeline->resolve(); }

void Universe::Tick() {
  DEBUG_VERBOSE("logic::quantum::Universe::Tick");
  if (logic::Event::HasEvents()) {
    std::string event_id = logic::Event::Get();
    DEBUG_INFO("");
    DEBUG_INFO("############################################################");
    DEBUG_INFO("# Event: %s", event_id.c_str());
    DEBUG_INFO("############################################################");
    start_timeline->process_event(event_id);
    start_timeline->resolve();
  }
}

Timeline *Universe::start_timeline = nullptr;
} // namespace quantum
} // namespace logic