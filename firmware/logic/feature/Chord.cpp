#include "Chord.h"

#include "../../config/key/Keycode.h"
#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "Key.h"

#include <memory>
#include <sstream>

#define DEFAULT_CHORD_DELAY 120

namespace logic {
namespace feature {

void Chord::OnPress(logic::quantum::Timeline &timeline,
                    const std::string &switch_uid,
                    const std::vector<std::string> &definition) {
  std::vector<std::string> chord_switches_uid;
  chord_switches_uid.push_back(switch_uid.substr(switch_uid.find('.') + 1));

  std::vector<std::string> switches_uid;
  for (auto item : definition) {
    chord_switches_uid.push_back(item);
    switches_uid.push_back(item);
  }
  std::string key_definition = switches_uid.back();
  chord_switches_uid.pop_back();
  switches_uid.pop_back();

  // Send timer event to the new timeline
  int delay_ms = Chord::delay_ms;
  // if (definition.size() > 2) {
  //   delay_ms = std::stoi(definition[2]);
  // }

  std::ostringstream switches_ss;
  for (auto item : switches_uid) {
    switches_ss << "." << item;
    // TODO: ignore chord if is uses a switch that is already pressed
  }

  std::string chord_id = "chord" + switch_uid.substr(6) + switches_ss.str();

  DEBUG_INFO("logic::feature::Chord::OnPress %s", chord_id.c_str());

  // Execute key
  Key::Get(key_definition)(timeline, chord_id);
  quantum::Timeline::Ptr timeline_chord = timeline.get_children().back();
  timeline_chord->set_complexity(switches_uid.size() + 1);
  timeline_chord->set_name(timeline_chord->name + "." + chord_id);

  // Handle timer
  std::string timer_event_id = chord_id + ".timer";
  timeline_chord->add_timer(timer_event_id, delay_ms);
  ActionFuncPtr chord_action(new ActionFunc(
      [](quantum::Timeline &timeline) { timeline.stop_timers(); }));
  timeline_chord->add_commit_action(chord_action);

  // Add actions for the other switches in the Chord
  for (auto switch_uid : switches_uid) {
    std::string event = "switch." + switch_uid + ".pressed";
    ActionFuncPtr combo_action(
        new ActionFunc([event, chord_id, chord_switches_uid,
                        timer_event_id](quantum::Timeline &timeline) {
          timeline.process_combo_event(event, chord_id, chord_switches_uid,
                                       timer_event_id);
        }));
    timeline_chord->add_combo_event(event, combo_action);
  }
}

int Chord::delay_ms = DEFAULT_CHORD_DELAY;
} // namespace feature
} // namespace logic