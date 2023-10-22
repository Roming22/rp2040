#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

class Keyboard {
private:
  static Keyboard *instance;
  // Singleton
  Keyboard(){};

public:
  // Singleton
  Keyboard(const Keyboard &obj) = delete;
  Keyboard &operator=(const Keyboard &obj) = delete;

  static void Setup();

  static void Tick();
};
#endif