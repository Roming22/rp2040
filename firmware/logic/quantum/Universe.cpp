#include "Universe.h"

#include "../../utils/Debug.hpp"
#include "../../utils/Fps.h"
#include "../../utils/Memory.h"
#include "../../utils/Time.h"
#include "../Events.h"
#include "../feature/Layer.h"
#include "Timeline.h"

namespace logic {
namespace quantum {
void Universe::Setup(std::string layer_name) {
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Universe::Setup %s", layer_name.c_str());
  // Activates the default layer.
  DEBUG_INFO("Universe: Loading %s", layer_name.c_str());
  start_timeline = Timeline::New(layer_name);
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

void Universe::Tick() {
  DEBUG_VERBOSE("logic::quantum::Universe::Tick");
  utils::FPS::Tick("Universe");
  if (logic::Event::HasEvents()) {
    Event::Ptr event = logic::Event::Get();
    Timeline *timeline = start_timeline.get();
    if (event->timeline != nullptr) {
      timeline = event->timeline;
    }
    DEBUG_INFO("");
    DEBUG_INFO("############################################################");
    DEBUG_INFO("# @%dms Universe Event %s for %d", event->time,
               event->id.c_str(), timeline);
    DEBUG_INFO("############################################################");
    utils::Memory::PrintMemoryUsage();
    timeline->process_event(event->id);
    start_timeline->resolve();
  }
}

Timeline::Ptr Universe::start_timeline = nullptr;
} // namespace quantum
} // namespace logic