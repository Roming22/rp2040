#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

#include <memory>

namespace firmware {
class Keyboard {
public:
  typedef std::shared_ptr<Keyboard> Ptr;

private:
  // Singleton
  static Ptr instance;
  Keyboard(){};

  static Ptr New();

public:
  // Singleton
  Keyboard(const Keyboard &obj) = delete;
  Keyboard &operator=(const Keyboard &obj) = delete;

  static void Setup();

  static void Tick();
};
} // namespace firmware
#endif