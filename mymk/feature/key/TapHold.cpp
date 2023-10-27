#include "TapHold.h"
#include "../../utils/Debug.hpp"

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
  DEBUG_VERBOSE("TapHold::LoadNo");
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
  const std::string timeline_id = "multitap";
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  Timeline &new_timeline = timeline.split(timeline_id);
  new_timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("TapHold::OnPress %s", switch_uid.c_str());
  new_timeline.add_commit_action([definition](Timeline &timeline) {
    TapHold::OnCommit(timeline, definition);
  });

  // On release configuration
  new_timeline.add_event_function(
      release_event, [switch_uid, definition](Timeline &timeline) {
        TapHold::OnRelease(timeline, switch_uid, definition);
      });
}

void TapHold::OnCommit(Timeline &timeline,
                       const std::vector<std::string> &definition) {
  DEBUG_INFO("TapHold::OnCommit");
}

void TapHold::OnRelease(Timeline &timeline, const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("TapHold::OnRelease");
  DEBUG_INFO("TapHold::OnRelease %s", switch_uid.c_str());
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
