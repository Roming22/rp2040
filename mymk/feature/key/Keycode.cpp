#include "Keycode.h"
#include "../../hardware/usb/UsbKey.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

#include <functional>
#include <string>
#include <vector>

std::function<void(Timeline &)>
Keycode::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("Keycode::LoadDefinition");
  const std::string keycode = definition[0];
  return [switch_uid, keycode](Timeline &timeline) {
    const std::string timeline_id = "keycode." + keycode;
    Timeline &new_timeline = timeline.split(timeline_id);
    OnPress(new_timeline, switch_uid, keycode);
  };
}

void Keycode::OnPress(Timeline &timeline, const std::string &switch_uid,
                      const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnPress");
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");
  timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("Keycode::OnPress %s: %s", switch_uid.c_str(), keycode.c_str());
  timeline.add_commit_action(
      [keycode](Timeline &timeline) { Keycode::OnCommit(timeline, keycode); });

  // On release configuration
  timeline.add_event_function(
      release_event, [switch_uid, keycode](Timeline &timeline) {
        Keycode::OnRelease(timeline, switch_uid, keycode);
      });
}

void Keycode::OnCommit(Timeline &timeline, const std::string &keycode) {
  DEBUG_INFO("Keycode pressed: %s", keycode.c_str());
  UsbKey::Press(keycode);
}

void Keycode::OnRelease(Timeline &timeline, const std::string &switch_uid,
                        const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnRelease");
  timeline.add_commit_action([keycode](Timeline &) {
    DEBUG_INFO("Keycode released: %s", keycode.c_str());
    UsbKey::Release(keycode);
  });
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
