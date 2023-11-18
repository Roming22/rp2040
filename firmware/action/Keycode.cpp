#include "Keycode.h"
#include "../hardware/usb/Key.h"
#include "../logic/quantum/Timeline.h"
#include "../utils/Debug.hpp"

#include <string>

namespace action {
void Keycode::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::string &keycode) {
  DEBUG_VERBOSE("action::Keycode::OnPress");
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");
  timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("action::Keycode::OnPress %s: %s", switch_uid.c_str(),
             keycode.c_str());
  timeline.add_commit_action([keycode](logic::quantum::Timeline &timeline) {
    Keycode::OnCommit(timeline, keycode);
  });

  // On release configuration
  timeline.add_event_function(
      release_event, [switch_uid, keycode](logic::quantum::Timeline &timeline) {
        Keycode::OnRelease(timeline, switch_uid, keycode);
      });
}

void Keycode::OnCommit(logic::quantum::Timeline &timeline,
                       const std::string &keycode) {
  DEBUG_INFO("action::Keycode pressed: %s", keycode.c_str());
  hardware::usb::Key::Press(keycode);
}

void Keycode::OnRelease(logic::quantum::Timeline &timeline,
                        const std::string &switch_uid,
                        const std::string &keycode) {
  DEBUG_VERBOSE("action::Keycode::OnRelease");
  timeline.add_commit_action([keycode](logic::quantum::Timeline &) {
    DEBUG_INFO("Keycode released: %s", keycode.c_str());
    hardware::usb::Key::Release(keycode);
  });
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
} // namespace action