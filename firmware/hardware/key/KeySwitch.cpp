#include "../../utils/Debug.hpp"
#include "KeySwitch.h"

namespace hardware {
namespace key {
void KeySwitch::poll_events(std::vector<int> &events) {
  DEBUG_VERBOSE("Key.poll_events");
  DEBUG_ERROR("[ERROR] Key.poll_events() is not implemented");
  delay(3600000);
}
} // namespace key
} // namespace hardware