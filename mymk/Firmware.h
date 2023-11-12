#ifndef MYMK_KEYBOARD
#define MYMK_KEYBOARD

class Firmware {
private:
  static Firmware *instance;
  // Singleton
  Firmware(){};

public:
  // Singleton
  Firmware(const Firmware &obj) = delete;
  Firmware &operator=(const Firmware &obj) = delete;

  static void Setup();

  static void Tick();
};
#endif