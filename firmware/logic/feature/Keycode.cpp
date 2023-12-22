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
  logic::quantum::Timeline &timeline_key = timeline.split(timeline_id, 1);

  DEBUG_INFO("logic::feature::Keycode::OnPress %s", timeline_id.c_str());

  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On commit actions
  timeline_key.add_commit_action([definition](
                                     logic::quantum::Timeline &timeline) {
    DEBUG_INFO("logic::feature::Keycode pressed: %s", timeline.history.c_str());
    for (auto keycode : definition) {
      hardware::usb::Key::Press(keycode);
    }
  });

  // On release configuration
  timeline_key.set_event_action(
      release_event, [release_event, switch_uid,
                      definition](logic::quantum::Timeline &timeline) {
        DEBUG_INFO("logic::feature::Keycode::OnRelease %s",
                   timeline.history.c_str());
        timeline.add_commit_action(
            [definition](logic::quantum::Timeline &timeline) {
              DEBUG_INFO("logic::feature::Keycode released %s",
                         timeline.history.c_str());
              for (auto keycode : definition) {
                hardware::usb::Key::Release(keycode);
              }
            });
        timeline.remove_event_action(release_event);
      });
}
} // namespace feature
} // namespace logic