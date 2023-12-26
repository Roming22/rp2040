#include "BaseBoard.h"

#include "../../utils/Debug.hpp"
#include "../../utils/Fps.h"

namespace hardware {
namespace board {
void BaseBoard::load_switch_events() {
  // DEBUG_VERBOSE("harware::board::BaseBoard::load_switch_events");
  key_switch->poll_events(switch_events);
}

void BaseBoard::tick() {
  // DEBUG_ERROR("[ERROR] BaseBoard::tick is not implemented");
  delay(3600000);
}

void BaseBoard::Tick() {
  // DEBUG_VERBOSE("harware::board::BaseBoard::Tick");
  if (instance->is_connected) {
    hardware::txrx::BitBang::Tick();
  }
  instance->tick();
}

BaseBoard::Ptr BaseBoard::instance = nullptr;
} // namespace board
} // namespace hardware