#include "MultiTap.h"
#include "../../utils/Debug.hpp"

std::function<void(Timeline &)>
MultiTap::LoadDefinition(const std::string &switch_uid,
                         const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("MultiTap::Load");
  return [switch_uid, definition](Timeline &timeline) {
    OnPress(timeline, switch_uid, definition);
  };
}

void MultiTap::OnPress(Timeline &timeline, const std::string &switch_uid,
                       const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("MultiTap::OnPress");
  const std::string timeline_id = "multitap";
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  Timeline &new_timeline = timeline.split(timeline_id);
  new_timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("MultiTap::OnPress %s", switch_uid.c_str());
  new_timeline.add_commit_action([definition](Timeline &timeline) {
    MultiTap::OnCommit(timeline, definition);
  });

  // On release configuration
  new_timeline.add_event_function(
      release_event, [switch_uid, definition](Timeline &timeline) {
        MultiTap::OnRelease(timeline, switch_uid, definition);
      });
}

void MultiTap::OnCommit(Timeline &timeline,
                        const std::vector<std::string> &definition) {
  DEBUG_INFO("MultiTap::OnCommit");
}

void MultiTap::OnRelease(Timeline &timeline, const std::string &switch_uid,
                         const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("MultiTap::OnRelease");
  DEBUG_INFO("MultiTap::OnRelease %s", switch_uid.c_str());
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}
