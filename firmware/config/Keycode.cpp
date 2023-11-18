#include "Keycode.h"
#include "../action/Keycode.h"
#include "../hardware/usb/Key.h"
#include "../logic/quantum/Timeline.h"
#include "../utils/Debug.hpp"

#include <functional>
#include <string>
#include <vector>

namespace config {
KeyFunc Keycode::Load(const std::vector<std::string> &definition) {
  DEBUG_DEBUG("config::Keycode::Load: %s", definition[0].c_str());
  const std::string keycode = definition[0];
  return [keycode](logic::quantum::Timeline &timeline, std::string switch_uid) {
    const std::string timeline_id = "keycode." + keycode;
    logic::quantum::Timeline &new_timeline = timeline.split(timeline_id);
    action::Keycode::OnPress(new_timeline, switch_uid, keycode);
  };
}
} // namespace config