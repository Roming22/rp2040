#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

#include "config/Loader.hpp"

class Keyboard {
private:
  // Singleton
  Keyboard() { load_config(); };

public:
  // Singleton
  Keyboard(const Keyboard &obj) = delete;
  Keyboard &operator=(const Keyboard &obj) = delete;
  static Keyboard *Get(const int pin = 0, const int count = 0) {
    static Keyboard *instance = new Keyboard();
    return instance;
  }

  static void Setup() { Get(); }

  static void Loop() { BaseBoard::Loop(); }
};
#endif