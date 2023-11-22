#include "KeySwitch.h"
#include "../../utils/Debug.hpp"

namespace hardware {
namespace key {
void KeySwitch::poll_events(std::vector<int> &events) {
  DEBUG_VERBOSE("harware::key::KeySwitch::poll_events");
  DEBUG_ERROR("[ERROR] Key.poll_events() is not implemented");
  delay(3600000);
}
} // namespace key
} // namespace hardware