#include "Keycode.h"
#include "../../hardware/usb/Key.h"
#include "../../utils/Debug.hpp"
#include "../quantum/Timeline.h"

#include <string>

namespace logic {
namespace feature {
void Keycode::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("logic::feature::Keycode::OnPress");
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On commit actions
  DEBUG_INFO("logic::feature::Keycode::OnPress %s: %s", switch_uid.c_str(),
             definition[0].c_str());
  timeline.add_commit_action([definition](logic::quantum::Timeline &timeline) {
    Keycode::OnCommit(timeline, definition);
  });

  // On release configuration
  timeline.set_event_action(
      release_event,
      [switch_uid, definition](logic::quantum::Timeline &timeline) {
        Keycode::OnRelease(timeline, switch_uid, definition);
      });
}

void Keycode::OnCommit(logic::quantum::Timeline &timeline,
                       const std::vector<std::string> &definition) {
  DEBUG_INFO("logic::feature::Keycode pressed: %s", definition[0].c_str());
  for (auto keycode : definition) {
    hardware::usb::Key::Press(keycode);
  }
}

void Keycode::OnRelease(logic::quantum::Timeline &timeline,
                        const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("logic::feature::Keycode::OnRelease");
  timeline.add_commit_action([definition](logic::quantum::Timeline &) {
    DEBUG_INFO("logic::feature::Keycode released: %s", definition[0].c_str());
    for (auto keycode : definition) {
      hardware::usb::Key::Release(keycode);
    }
  });
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_action(release_event);
}
} // namespace feature
} // namespace logic