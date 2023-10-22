#include "BaseBoard.h"
#include "../../utils/Debug.hpp"

void BaseBoard::load_switch_events(std::vector<int> &switch_events) {
  DEBUG_VERBOSE("BaseBoard::load_switch_events");
  key->poll_events(switch_events);
}

void BaseBoard::tick() {
  DEBUG_ERROR("[ERROR] BaseBoard::tick is not implemented");
  delay(3600000);
}

void BaseBoard::Tick() {
  DEBUG_VERBOSE("BaseBoard::Tick");
  if (instance->is_connected) {
    BitBang::Tick();
  }
  instance->tick();
}

BaseBoard *BaseBoard::instance = nullptr;
