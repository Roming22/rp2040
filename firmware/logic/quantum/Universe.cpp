#include "Universe.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Memory.h"
#include "../../utils/Time.h"
#include "../Events.h"
#include "../feature/Layer.h"
#include "Timeline.h"

#include <memory>
#include <string>

namespace logic {
namespace quantum {
void Universe::Setup(std::string layer_name) {
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Universe::Setup %s", layer_name.c_str());
  // Activates the default layer.
  DEBUG_INFO("Universe: Loading %s", layer_name.c_str());
  start_timeline = Timeline::Ptr(new Timeline(layer_name));
  std::string switch_id("switch.1");
  logic::feature::Layer::OnPress(layer_name, *start_timeline, switch_id, true);
  start_timeline->resolve();
  std::string release_event = switch_id + ".released";
  start_timeline->process_event(release_event);
}

void Universe::StartTimeline(Timeline::Ptr &timeline) {
  DEBUG_INFO("logic::quantum::Universe::StartTimeline %s",
             timeline->name.c_str());
  start_timeline = timeline;
}

void Universe::Resolve() { start_timeline->resolve(); }

void Universe::Tick() {
  DEBUG_VERBOSE("logic::quantum::Universe::Tick");
  while (logic::Event::HasEvents()) {
    std::string event_id = logic::Event::Get();
    DEBUG_INFO("");
    DEBUG_INFO("############################################################");
    DEBUG_INFO("# @%dms Universe Event %s for %d", utils::Time::Now(),
               event_id.c_str(), start_timeline.get());
    DEBUG_INFO("############################################################");
    utils::Memory::PrintMemoryUsage();
    start_timeline->process_event(event_id);
    start_timeline->resolve();
  }
}

Timeline::Ptr Universe::start_timeline = nullptr;
} // namespace quantum
} // namespace logic