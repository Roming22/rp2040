#include "Keycode.h"
#include "../../logic/feature/Keycode.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

namespace config {
namespace key {
logic::KeyFunc Keycode::Load(const std::vector<std::string> &definition) {
  DEBUG_DEBUG("config::key::Keycode::Load: %s", definition[0].c_str());
  return
      [definition](logic::quantum::Timeline &timeline, std::string switch_uid) {
        const std::string timeline_id = "keycode." + definition[0];
        logic::quantum::Timeline &new_timeline = timeline.split(timeline_id, 1);
        logic::feature::Keycode::OnPress(new_timeline, switch_uid, definition);
      };
}
} // namespace key
} // namespace config