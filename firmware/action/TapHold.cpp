#include "TapHold.h"
#include "../logic/Timer.h"
#include "../utils/Debug.hpp"
#include "KeyParser.h"

#include <functional>
#include <string>
#include <vector>

namespace action {
std::function<void(logic::quantum::Timeline &)>
TapHold::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition,
                        const InterruptMode interrupt_mode) {
  DEBUG_VERBOSE("TapHold::Load");
  return [switch_uid, definition,
          interrupt_mode](logic::quantum::Timeline &timeline) {
    OnPress(timeline, switch_uid, definition, interrupt_mode);
  };
}

std::function<void(logic::quantum::Timeline &)>
TapHold::LoadHoldDefinition(const std::string &switch_uid,
                            const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadHold");
  return LoadDefinition(switch_uid, definition, HOLD);
}

std::function<void(logic::quantum::Timeline &)>
TapHold::LoadNoneDefinition(const std::string &switch_uid,
                            const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadNone");
  return LoadDefinition(switch_uid, definition, NONE);
}

std::function<void(logic::quantum::Timeline &)>
TapHold::LoadTapDefinition(const std::string &switch_uid,
                           const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::LoadTap");
  return LoadDefinition(switch_uid, definition, TAP);
}

void TapHold::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition,
                      const InterruptMode interrupt_mode) {
  DEBUG_VERBOSE("TapHold::OnPress");

  std::string definition_tap = definition[0];
  std::string definition_interrupt;
  std::string definition_hold = definition[1];

  switch (interrupt_mode) {
  case HOLD:
    definition_interrupt = definition_hold;
    break;
  case NONE:
    definition_interrupt = "NONE";
    break;
  case TAP:
    definition_interrupt = definition_tap;
    break;
  }

  logic::quantum::Timeline &timeline_tap =
      timeline.split(switch_uid + ".taphold.tap");
  logic::quantum::Timeline &timeline_interrupt =
      timeline.split(switch_uid + ".taphold.interrupt");
  logic::quantum::Timeline &timeline_hold =
      timeline.split(switch_uid + ".taphold.hold");

  std::string release_event = switch_uid + ".released";

  // Send timer event to the new timelines
  int delay_ms = 1000;
  if (definition.size() > 2) {
    delay_ms = std::stoi(definition[2]);
  }
  std::string timer_event_id = switch_uid + ".taphold.timer";
  logic::Timer::Start(timer_event_id, delay_ms);

  // Tap logic::quantum::Timeline
  timeline_tap.add_event_function(
      release_event, [switch_uid, release_event, definition_tap,
                      timer_event_id](logic::quantum::Timeline &timeline_tap) {
        DEBUG_VERBOSE("Lambda tap: %s", switch_uid.c_str());
        KeyParser::Load(switch_uid, definition_tap)(timeline_tap);
        DEBUG_INFO("Trigger release?");
        timeline_tap.add_commit_action(
            [release_event](logic::quantum::Timeline &timeline_tap) {
              timeline_tap.process_event(release_event);
            });
        DEBUG_INFO("Trigger release?");
        logic::Timer::Stop(timer_event_id);
        timeline_tap.remove_event_function("interrupt");
      });
  timeline_tap.add_event_function("interrupt", &logic::quantum::Timeline::End);
  timeline_tap.add_event_function(timer_event_id,
                                  &logic::quantum::Timeline::End);

  // Interrupt logic::quantum::Timeline
  timeline_interrupt.add_event_function(release_event,
                                        &logic::quantum::Timeline::End);
  timeline_interrupt.add_event_function(
      "interrupt", [switch_uid, definition_interrupt, timer_event_id](
                       logic::quantum::Timeline &timeline_interrupt) {
        DEBUG_VERBOSE("Lambda interrupt: %s", switch_uid.c_str());
        KeyParser::Load(switch_uid, definition_interrupt)(timeline_interrupt);
        logic::Timer::Stop(timer_event_id);
        timeline_interrupt.remove_event_function("interrupt");
      });
  timeline_interrupt.add_event_function(timer_event_id,
                                        &logic::quantum::Timeline::End);

  // Hold logic::quantum::Timeline
  timeline_hold.add_event_function(release_event,
                                   &logic::quantum::Timeline::End);
  timeline_hold.add_event_function("interrupt", &logic::quantum::Timeline::End);
  timeline_hold.add_event_function(
      timer_event_id, [switch_uid, definition_hold, timer_event_id](
                          logic::quantum::Timeline &timeline_hold) {
        DEBUG_VERBOSE("Lambda hold: %s", switch_uid.c_str());
        KeyParser::Load(switch_uid, definition_hold)(timeline_hold);
        timeline_hold.remove_event_function("interrupt");
        timeline_hold.remove_event_function(timer_event_id);
      });
}
} // namespace action