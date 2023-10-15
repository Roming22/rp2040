#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

#include "config/Loader.hpp"
#include "hardware/board/BaseBoard.hpp"
#include "logic/Timer.hpp"
#include "logic/quantum/Universe.hpp"
#include "utils/Fps.hpp"
#include "utils/Time.hpp"

class Keyboard {
private:
  static Keyboard *instance;
  // Singleton
  Keyboard(){};

public:
  // Singleton
  Keyboard(const Keyboard &obj) = delete;
  Keyboard &operator=(const Keyboard &obj) = delete;

  static void Setup() {
    if (instance == nullptr) {
      instance = new Keyboard();
      load_config();
      Universe::Start();
    }
  }
  static void Tick() {
    DEBUG_VERBOSE("Keyboard::Tick");
    Time::Tick();
    Timer::Tick();
    BaseBoard::Tick();
    FPS::Tick();
  }
};
Keyboard *Keyboard::instance = nullptr;
#endif