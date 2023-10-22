#include "Key.h"
#include "../../utils/Debug.hpp"

void Key::poll_events(std::vector<int> &events) {
  DEBUG_VERBOSE("Key.poll_events");
  DEBUG_ERROR("[ERROR] Key.poll_events() is not implemented");
  delay(3600000);
}
