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
  static bool IsUsbConnected(int vusb_pins);
  static void ParseJson(JsonDocument &jsonDoc, const char *&jsonString);

public:
  static void Load();
};
} // namespace loader
} // namespace config
#endif