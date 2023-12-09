#ifndef MYMK_CONFIG_LOADER_KEYBOARD
#define MYMK_CONFIG_LOADER_KEYBOARD

#include <ArduinoJson.h>
#include <string>

namespace config {
namespace loader {
class Keyboard {
protected:
  static void LoadHardware();
  static void LoadLayout();
  static std::string GetControllerUid();
  static void ParseJson(DynamicJsonDocument &jsonDoc, const char *&jsonString);

public:
  static void Load();
};
} // namespace loader
} // namespace config
#endif