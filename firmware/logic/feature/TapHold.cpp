#include "TapHold.h"

#include "../../config/key/Keycode.h"
#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "Key.h"

#define DEFAULT_TAPHOLD_DELAY 250

namespace logic {
namespace feature {

int TapHold::delay_ms = DEFAULT_TAPHOLD_DELAY;

std::shared_ptr<std::function<void()>> fixme00;

void TapHold::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("logic::feature::TapHold::OnPress");
  const std::string release_event = switch_uid + ".released";
  const std::string timer_event_id = switch_uid + ".taphold.timer";
  int delay_ms = TapHold::delay_ms;
  if (definition.size() > 2) {
    delay_ms = std::stoi(definition[2]);
  }

  // Tap timeline
  Key::Get(definition[0])(timeline, switch_uid);
  logic::quantum::Timeline *timeline_tap =
      timeline.get_children().rbegin()->get();
  timeline_tap->set_complexity(2);
  timeline_tap->set_name(timeline_tap->name + ".tap");

  // Hold timeline
  Key::Get(definition[1])(timeline, switch_uid);
  logic::quantum::Timeline *timeline_hold =
      timeline.get_children().rbegin()->get();
  timeline_hold->set_complexity(2);
  timeline_hold->set_name(timeline_hold->name + ".hold");
  timeline_hold->add_timer(timer_event_id, delay_ms);

  // Timeline selection
  DEBUG_INFO("timelines %d & %d", timeline_tap, timeline_hold);
  const ActionFuncPtr release_action(
      NewActionFunc([timeline_hold](quantum::Timeline &) {
        DEBUG_INFO("About to prune %d: %s", timeline_hold,
                   timeline_hold->name.c_str());
        timeline_hold->prune();
        DEBUG_INFO("Pruned");
      }));
  timeline_tap->set_event_action(release_event, release_action);
  const ActionFuncPtr timer_action(
      NewActionFunc([timeline_tap](quantum::Timeline &) {
        DEBUG_INFO("About to prune %d: %s", timeline_tap,
                   timeline_tap->name.c_str());
        timeline_tap->prune();
        DEBUG_INFO("Pruned");
      }));
  timeline_hold->set_event_action(timer_event_id, timer_action);

  // TODO: Fix memory
  // There's an issue with memory management. Without this function,
  // the timeline_hold pointer valuer gets mangled (overwritten by something
  // else?) and the release_action function fails.
  fixme00 = std::shared_ptr<std::function<void()>>(
      new std::function<void()>([release_action, timeline_hold]() {
        DEBUG_INFO("TESTME TIMELINE %d: %s", timeline_hold,
                   timeline_hold->name.c_str());
        (*release_action)(*timeline_hold);
      }));
}
} // namespace feature
} // namespace logic