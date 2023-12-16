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

  logic::quantum::Timeline &timeline_taphold =
      timeline.split(switch_uid + ".taphold");

  // Send timer event to the new timelines
  int delay_ms = TapHold::delay_ms;
  if (definition.size() > 2) {
    delay_ms = std::stoi(definition[2]);
  }
  std::string timer_event_id = switch_uid + ".taphold.timer";

  std::string release_event = switch_uid + ".released";

  // Tap timeline
  Key::Get(definition[0])(timeline_taphold, switch_uid);
  logic::quantum::Timeline *timeline_tap =
      *timeline_taphold.get_children().rbegin();

  // Hold timeline
  Key::Get(definition[1])(timeline_taphold, switch_uid);
  logic::quantum::Timeline *timeline_hold =
      *timeline_taphold.get_children().rbegin();

  // Selector timeline
  logic::quantum::Timeline *timeline_selector =
      &timeline_taphold.split("timer");
  logic::Timer::Start(timer_event_id, delay_ms, *timeline_selector);
  timeline_selector->clear_events_action();
  timeline_selector->set_event_action("ignore_unknown_events", ActionFuncNoOp);
  timeline_selector->set_event_action(
      release_event, [timer_event_id, timeline_selector,
                      timeline_hold](logic::quantum::Timeline &timeline) {
        logic::Timer::Stop(timer_event_id);
        timeline_hold->end();
        timeline_selector->end();
      });
  timeline_selector->set_event_action(
      timer_event_id, [timer_event_id, timeline_selector,
                       timeline_tap](logic::quantum::Timeline &timeline_hold) {
        timeline_tap->end();
        timeline_selector->end();
      });
}

int TapHold::delay_ms = DEFAULT_TAPHOLD_DELAY;
} // namespace feature
} // namespace logic