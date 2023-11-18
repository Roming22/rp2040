#include "Keyboard.h"
#include "config/Loader.hpp"
#include "hardware/board/BaseBoard.h"
#include "logic/Timer.h"
#include "logic/quantum/Universe.h"
#include "utils/Debug.hpp"
#include "utils/Fps.h"
#include "utils/Time.h"

namespace firmware {
void Keyboard::Setup() {
  if (instance == nullptr) {
    instance = new Keyboard();
    load_config();
  }
}

void Keyboard::Tick() {
  DEBUG_VERBOSE("Keyboard::Tick");
  utils::Time::Tick();
  logic::Timer::Tick();
  hardware::board::BaseBoard::Tick();
  logic::quantum::Universe::Tick();
  utils::FPS::Tick();
}

Keyboard *Keyboard::instance = nullptr;
} // namespace firmware