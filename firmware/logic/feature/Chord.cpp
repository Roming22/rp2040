#include "Chord.h"

#include "../../config/key/Keycode.h"
#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "Key.h"
#include <sstream>

#define DEFAULT_CHORD_DELAY 50

namespace logic {
namespace feature {
void Chord::OnPress(logic::quantum::Timeline &timeline,
                    const std::string &switch_uid,
                    const std::vector<std::string> &definition) {
  std::vector<std::string> switches;
  for (auto item : definition) {
    switches.push_back(item);
  }
  std::string keycode = switches.back();
  switches.pop_back();
  std::ostringstream switches_ss;
  for (auto item : switches) {
    switches_ss << "." << item;
  }
  DEBUG_INFO("logic::feature::Chord::OnPress %s", keycode.c_str());

  logic::quantum::Timeline &timeline_chord = timeline.split(
      switch_uid + switches_ss.str() + ".chord", switches.size() + 1);

  // Send timer event to the new timeline
  int delay_ms = Chord::delay_ms;
  // if (definition.size() > 2) {
  //   delay_ms = std::stoi(definition[2]);
  // }
  std::string timer_event_id = switch_uid + switches_ss.str() + ".chord.timer";
  logic::Timer::Start(timer_event_id, delay_ms, timeline_chord);
}

int Chord::delay_ms = DEFAULT_CHORD_DELAY;
} // namespace feature
} // namespace logic