#ifndef MYMK_CONFIG_LOADER_LAYER
#define MYMK_CONFIG_LOADER_LAYER

#include "../../logic/feature/Layer.h"

#include <ArduinoJson.h> // Include the appropriate header for ArduinoJson (assuming it's used).
#include <string>

namespace config {
namespace loader {
class Layer {
protected:
  static void LoadLedColor(const JsonArray &config, int *color);
  static void LoadKeys(const JsonArray &config, logic::feature::KeyMap &keys);
  static void LoadCombos(const JsonObject &config,
                         logic::feature::KeyMap &keys);

public:
  static void Load(const std::string name, const JsonObject &config);
};
} // namespace loader
} // namespace config
#endif