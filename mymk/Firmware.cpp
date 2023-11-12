#include "Firmware.h"
#include "config/Loader.hpp"
#include "hardware/board/BaseBoard.h"
#include "logic/Timer.h"
#include "logic/quantum/Universe.h"
#include "utils/Debug.hpp"
#include "utils/Fps.h"
#include "utils/Time.h"

#include "Keyboard.h"

void Firmware::Setup() {
  if (instance == nullptr) {
    instance = new Firmware();
    load_config();
  }
  Keyboard.begin();
}

void Firmware::Tick() {
  DEBUG_VERBOSE("Firmware::Tick");
  Time::Tick();
  Timer::Tick();
  BaseBoard::Tick();
  Universe::Tick();
  FPS::Tick();
}

Firmware *Firmware::instance = nullptr;
