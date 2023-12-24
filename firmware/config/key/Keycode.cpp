#include "Keycode.h"

#include "../../logic/feature/Keycode.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

namespace config {
namespace key {
logic::KeyFunc Keycode::Load(const std::vector<std::string> &definition) {
  DEBUG_INFO("config::key::Keycode::Load: %s", definition[0].c_str());
  return
      [definition](logic::quantum::Timeline &timeline, std::string switch_uid) {
        logic::feature::Keycode::OnPress(timeline, switch_uid, definition);
      };
}
} // namespace key
} // namespace config