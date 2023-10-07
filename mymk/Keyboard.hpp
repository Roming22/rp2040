#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

#include "config/Loader.hpp"

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
      delay(1000);
    }
  }
  static void Loop() { BaseBoard::Loop(); }
};
Keyboard *Keyboard::instance = nullptr;
#endif