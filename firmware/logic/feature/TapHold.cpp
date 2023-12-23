#include "TapHold.h"
#include "../../config/key/Keycode.h"
#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "Key.h"
#include <string>
#include <vector>

#define DEFAULT_TAPHOLD_DELAY 250

namespace logic {
namespace feature {
void TapHold::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("logic::feature::TapHold::OnPress");

  logic::quantum::Timeline::Ptr timeline_taphold =
      timeline.split(switch_uid + ".taphold");

  // Send timer event to the new timelines
  int delay_ms = TapHold::delay_ms;
  if (definition.size() > 2) {
    delay_ms = std::stoi(definition[2]);
  }
  std::string timer_event_id = switch_uid + ".taphold.timer";

  std::string release_event = switch_uid + ".released";

  // Tap timeline
  Key::Get(definition[0])(*timeline_taphold, switch_uid);
  logic::quantum::Timeline::Ptr timeline_tap =
      *timeline_taphold->get_children().rbegin();

  // Hold timeline
  Key::Get(definition[1])(*timeline_taphold, switch_uid);
  logic::quantum::Timeline::Ptr timeline_hold =
      *timeline_taphold->get_children().rbegin();

  // Selector timeline
  logic::quantum::Timeline::Ptr timeline_selector =
      timeline_taphold->split("timer");
  timeline_taphold->add_timer(timer_event_id, delay_ms);
  timeline_selector->clear_events_action();
  timeline_selector->set_event_action("ignore_unknown_events", ActionFuncNoOp);
  ActionFuncPtr tap_action(
      new ActionFunc([timeline_hold](logic::quantum::Timeline &timeline) {
        timeline.stop_timers();
        timeline_hold->prune();
        timeline.prune();
      }));
  timeline_selector->set_event_action(release_event, tap_action);
  ActionFuncPtr hold_action(
      new ActionFunc([timer_event_id, timeline_selector,
                      timeline_tap](logic::quantum::Timeline &timeline_hold) {
        timeline_tap->prune();
        timeline_selector->prune();
      }));
  timeline_selector->set_event_action(timer_event_id, hold_action);
}

int TapHold::delay_ms = DEFAULT_TAPHOLD_DELAY;
} // namespace feature
} // namespace logic