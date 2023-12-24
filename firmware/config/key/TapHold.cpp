#include "TapHold.h"

#include "../../logic/feature/TapHold.h"
#include "../../utils/Debug.hpp"
#include "../loader/Key.h"

namespace config {
namespace key {
logic::KeyFunc TapHold::Load(const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("config::key::TapHold::Load");

  // Load keycodes recursively
  for (auto keycode : definition) {
    config::loader::Key::Load(keycode);
  }

  return
      [definition](logic::quantum::Timeline &timeline, std::string switch_uid) {
        logic::feature::TapHold::OnPress(timeline, switch_uid, definition);
      };
}
} // namespace key
} // namespace config