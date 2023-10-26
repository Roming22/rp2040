#include "Keycode.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

#include <string>

std::function<void(Timeline &)>
Keycode::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("Keycode::Load");
  const std::string keycode = definition[0];
  return [switch_uid, keycode](Timeline &timeline) {
    OnPress(timeline, switch_uid, keycode);
  };
}

void Keycode::OnPress(Timeline &timeline, const std::string &switch_uid,
                      const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnPress");
  const std::string timeline_id = "keycode." + keycode;
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  Timeline &new_timeline = timeline.split(timeline_id);
  new_timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("Keycode::OnPress %s: %s", switch_uid.c_str(), keycode.c_str());
  new_timeline.add_commit_action(
      [keycode](Timeline &timeline) { Keycode::OnCommit(timeline, keycode); });

  // On release configuration
  new_timeline.add_event_function(
      release_event, [switch_uid, keycode](Timeline &timeline) {
        Keycode::OnRelease(timeline, switch_uid, keycode);
      });
}

void Keycode::OnCommit(Timeline &timeline, const std::string &keycode) {
  DEBUG_INFO("Keycode::OnCommit Press: %s", keycode.c_str());
}

void Keycode::OnRelease(Timeline &timeline, const std::string &switch_uid,
                        const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnRelease");
  DEBUG_INFO("Keycode::OnRelease %s Release: %s", switch_uid.c_str(),
             keycode.c_str());
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
