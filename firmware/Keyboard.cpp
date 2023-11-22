#include "Keyboard.h"
#include "config/loader/Keyboard.h"
#include "hardware/board/BaseBoard.h"
#include "logic/Timer.h"
#include "logic/quantum/Universe.h"
#include "utils/Debug.hpp"
#include "utils/Fps.h"
#include "utils/Time.h"

namespace firmware {
void Keyboard::Setup() {
  DEBUG_VERBOSE("firmware::Keyboard::Setup");
  if (instance == nullptr) {
    instance = new Keyboard();
    config::file::Keyboard::Load();
  }
}

void Keyboard::Tick() {
  DEBUG_VERBOSE("firmware::Keyboard::Tick");
  utils::Time::Tick();
  logic::Timer::Tick();
  hardware::board::BaseBoard::Tick();
  logic::quantum::Universe::Tick();
  utils::FPS::Tick();
}

Keyboard *Keyboard::instance = nullptr;
} // namespace firmware