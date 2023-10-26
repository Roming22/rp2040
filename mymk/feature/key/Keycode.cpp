#include "Keycode.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

#include <string>

std::function<void()>
Keycode::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("Keycode::Load");
  const std::string keycode = definition[0];
  return [switch_uid, keycode]() { OnPress(switch_uid, keycode); };
}

void Keycode::OnPress(const std::string &switch_uid,
                      const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnPress");
  const std::string timeline_id = "keycode." + keycode;
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  Timeline &new_timeline = Timeline::GetCurrent().split(timeline_id);
  new_timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("Keycode::OnPress %s: %s", switch_uid.c_str(), keycode.c_str());
  new_timeline.add_commit_action(
      [switch_uid, keycode]() { Keycode::OnCommit(switch_uid, keycode); });

  // On release configuration
  new_timeline.add_event_function(release_event, [switch_uid, keycode]() {
    Keycode::OnRelease(switch_uid, keycode);
  });
}

void Keycode::OnCommit(const std::string &switch_uid,
                       const std::string &keycode) {
  DEBUG_INFO("Keycode::OnCommit %s: %s", switch_uid.c_str(), keycode.c_str());
}

void Keycode::OnRelease(const std::string &switch_uid,
                        const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnRelease");
  DEBUG_INFO("Keycode::OnRelease %s: %s", switch_uid.c_str(), keycode.c_str());
  Timeline &timeline = Timeline::GetCurrent();
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
