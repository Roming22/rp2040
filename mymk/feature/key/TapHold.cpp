#include "TapHold.h"
#include "../../logic/Timer.h"
#include "../../utils/Debug.hpp"

#include <functional>
#include <string>
#include <vector>

std::function<void(Timeline &)>
TapHold::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition,
                        const InterruptMode interrupt_mode) {
  DEBUG_VERBOSE("TapHold::Load");
  return [switch_uid, definition](Timeline &timeline) {
    OnPress(timeline, switch_uid, definition);
  };
}

std::function<void(Timeline &)>
TapHold::LoadHoldDefinition(const std::string &switch_uid,
                            const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadHold");
  return LoadDefinition(switch_uid, definition, HOLD);
}

std::function<void(Timeline &)>
TapHold::LoadNoneDefinition(const std::string &switch_uid,
                            const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadNone");
  return LoadDefinition(switch_uid, definition, NONE);
}

std::function<void(Timeline &)>
TapHold::LoadTapDefinition(const std::string &switch_uid,
                           const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadTap");
  return LoadDefinition(switch_uid, definition, TAP);
}

void TapHold::OnPress(Timeline &timeline, const std::string &switch_uid,
                      const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::OnPress");

  Timeline &timeline_tap = timeline.split(switch_uid + ".taphold.tap");
  Timeline &timeline_interrupt =
      timeline.split(switch_uid + ".taphold.interrupt");
  Timeline &timeline_hold = timeline.split(switch_uid + ".taphold.hold");

  std::string release_event = switch_uid + ".released";

  // Send timer event to the new timelines
  int delay_ms = 1000;
  std::string timer_event_id = switch_uid + ".taphold.timer";
  Timer::Start(timer_event_id, delay_ms);

  // Tap Timeline
  timeline_tap.add_commit_action([switch_uid](Timeline &timeline_tap) {
    DEBUG_INFO("Tap %s", switch_uid.c_str());
    // TODO: Activate "on press" Tap definition
  });
  timeline_tap.add_event_function(
      release_event, [release_event, timer_event_id](Timeline &timeline_tap) {
        DEBUG_VERBOSE("Lambda tap: %s", release_event.c_str());
        // TODO: Load "on release" Tap definition
        Timer::Stop(timer_event_id);
        timeline_tap.mark_determined();
        timeline_tap.remove_event_function(release_event);
      });
  timeline_tap.add_event_function("interrupt", &Timeline::End);
  timeline_tap.add_event_function(timer_event_id, &Timeline::End);

  // Interrupt Timeline
  timeline_interrupt.add_commit_action(
      [switch_uid](Timeline &timeline_interrupt) {
        DEBUG_INFO("Interrupt %s", switch_uid.c_str());
        // TODO: Activate "on press" Interrupt definition
      });
  timeline_interrupt.add_event_function(release_event, &Timeline::End);
  timeline_interrupt.add_event_function(
      "interrupt",
      [release_event, timer_event_id](Timeline &timeline_interrupt) {
        DEBUG_VERBOSE("Lambda interrupt: %s", release_event.c_str());
        // TODO: Load "on release" Interrupt definition
        // Reset interrupt
        Timer::Stop(timer_event_id);
        timeline_interrupt.add_event_function(
            release_event, [](Timeline &timeline_interrupt) {});
        timeline_interrupt.mark_determined();
        timeline_interrupt.remove_event_function("interrupt");
      });
  timeline_interrupt.add_event_function(timer_event_id, &Timeline::End);

  // Hold Timeline
  timeline_hold.add_commit_action([switch_uid](Timeline &timeline_hold) {
    DEBUG_INFO("Hold %s", switch_uid.c_str());
    // TODO: Activate "on press" Hold definition
  });
  timeline_hold.add_event_function(release_event, &Timeline::End);
  timeline_hold.add_event_function("interrupt", &Timeline::End);
  timeline_hold.add_event_function(
      timer_event_id, [release_event, timer_event_id](Timeline &timeline_hold) {
        DEBUG_VERBOSE("Lambda hold: %s", release_event.c_str());
        // TODO: Load "on release" Hold definition
        timeline_hold.add_event_function(release_event,
                                         [](Timeline &timeline_hold) {});
        timeline_hold.add_event_function("interrupt",
                                         [](Timeline &timeline_interrupt) {});
        timeline_hold.mark_determined();
        timeline_hold.remove_event_function(timer_event_id);
      });
}
