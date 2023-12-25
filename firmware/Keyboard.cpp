#include "Keyboard.h"

#include "config/loader/Keyboard.h"
#include "hardware/board/BaseBoard.h"
#include "logic/Timer.h"
#include "logic/quantum/Universe.h"
#include "utils/Debug.hpp"
#include "utils/Fps.h"
#include "utils/Time.h"

namespace firmware {
Keyboard::Ptr Keyboard::instance = nullptr;

Keyboard::Ptr Keyboard::New() { return Ptr(new Keyboard()); }

void Keyboard::Setup() {
  DEBUG_VERBOSE("firmware::Keyboard::Setup");
  if (instance == nullptr) {
    instance = New();
    config::loader::Keyboard::Load();
  }
}

void Keyboard::Tick() {
  DEBUG_VERBOSE("firmware::Keyboard::Tick");
  // Move clock forward
  utils::Time::Tick();
  utils::FPS::Tick("Keyboard");
  // Get events
  logic::Timer::Tick();
  hardware::board::BaseBoard::Tick();
  // Process events
#ifndef MULTICORE
  logic::quantum::Universe::Tick();
#endif
}

} // namespace firmware