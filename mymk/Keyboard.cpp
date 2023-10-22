#include "Keyboard.h"
#include "config/Loader.hpp"
#include "hardware/board/BaseBoard.h"
#include "logic/Timer.h"
#include "logic/quantum/Universe.h"
#include "utils/Debug.hpp"
#include "utils/Fps.h"
#include "utils/Time.h"

void Keyboard::Setup() {
  if (instance == nullptr) {
    instance = new Keyboard();
    load_config();
  }
}

void Keyboard::Tick() {
  DEBUG_VERBOSE("Keyboard::Tick");
  Time::Tick();
  Timer::Tick();
  BaseBoard::Tick();
  Universe::Tick();
  FPS::Tick();
}

Keyboard *Keyboard::instance = nullptr;
