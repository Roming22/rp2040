#include "Chord.h"

#include "../../logic/feature/Chord.h"
#include "../../utils/Debug.hpp"
#include "../loader/Key.h"

namespace config {
namespace key {
logic::KeyFunc Chord::Load(const std::vector<std::string> &definition) {
  std::string keycode = definition.back();
  DEBUG_DEBUG("config::key::Chord::Load %s", keycode.c_str());
  config::loader::Key::Load(keycode);
  return
      [definition](logic::quantum::Timeline &timeline, std::string switch_uid) {
        logic::feature::Chord::OnPress(timeline, switch_uid, definition);
      };
}
} // namespace key
} // namespace config