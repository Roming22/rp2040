#include "Keycode.h"

#include "../../hardware/usb/Key.h"
#include "../../utils/Debug.hpp"
#include "../quantum/Timeline.h"

#include <sstream>

namespace logic {
namespace feature {
void Keycode::OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition) {
  std::ostringstream definition_ss;
  for (auto item : definition) {
    definition_ss << "." << item;
  }

  const std::string timeline_id = "keycode" + definition_ss.str();
  logic::quantum::Timeline::Ptr timeline_key = timeline.split(timeline_id);

  DEBUG_INFO("logic::feature::Keycode::OnPress %s", timeline_id.c_str());

  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On commit actions
  ActionFuncPtr commit_action(
      NewActionFunc([definition](logic::quantum::Timeline &timeline) {
        DEBUG_INFO("logic::feature::Keycode pressed: %s",
                   timeline.name.c_str());
        for (auto keycode : definition) {
          hardware::usb::Key::Press(keycode);
        }
      }));
  timeline_key->add_commit_action(commit_action);

  // On release configuration
  ActionFuncPtr release_action(
      NewActionFunc([release_event, switch_uid,
                     definition](logic::quantum::Timeline &timeline) {
        DEBUG_INFO("logic::feature::Keycode::OnRelease %s",
                   timeline.name.c_str());
        timeline.add_commit_action(
            NewActionFunc([definition](logic::quantum::Timeline &timeline) {
              DEBUG_INFO("logic::feature::Keycode released: %s",
                         timeline.name.c_str());
              for (auto keycode : definition) {
                hardware::usb::Key::Release(keycode);
              }
            }));
      }));
  timeline_key->set_release_action(release_event, release_action);
}
} // namespace feature
} // namespace logic