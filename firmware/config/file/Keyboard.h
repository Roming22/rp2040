#ifndef MYMK_CONFIG_FILE_KEYBOARD
#define MYMK_CONFIG_FILE_KEYBOARD

#include <ArduinoJson.h>
#include <string>

namespace config {
namespace file {
class Keyboard {
protected:
  static void LoadHardware();
  static void LoadLayout();
  static std::string GetControllerUid();
  static void ParseJson(DynamicJsonDocument &jsonDoc, const char *&jsonString);

public:
  static void Load();
};
} // namespace file
} // namespace config
#endif