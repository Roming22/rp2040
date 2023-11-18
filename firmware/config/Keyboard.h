#ifndef MYMK_CONFIG_KEYBOARD
#define MYMK_CONFIG_KEYBOARD

#include <ArduinoJson.h>
#include <string>

namespace config {
class Keyboard {
protected:
  static void LoadHardware();
  static void LoadLayout();
  static std::string GetControllerUid();
  static void ParseJson(DynamicJsonDocument &jsonDoc, const char *&jsonString);

public:
  static void Load();
};
} // namespace config
#endif