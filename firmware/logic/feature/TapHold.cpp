#include "TapHold.h"

#include "../../config/key/Keycode.h"
#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "Key.h"

#define DEFAULT_TAPHOLD_DELAY 250

namespace logic {
namespace feature {

int TapHold::delay_ms = DEFAULT_TAPHOLD_DELAY;

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

  // Timeline selection
  DEBUG_INFO("timelines %d & %d", timeline_tap, timeline_hold);
  const ActionFuncPtr release_action(NewActionFunc(
      [timeline_hold](quantum::Timeline &) { timeline_hold->prune(); }));
  timeline_tap->set_release_action(release_event, release_action);
  timeline_tap->add_timer(timer_event_id, delay_ms);
}
} // namespace feature
} // namespace logic