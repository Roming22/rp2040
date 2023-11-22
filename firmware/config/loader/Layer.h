#ifndef MYMK_CONFIG_LOADER_LAYER
#define MYMK_CONFIG_LOADER_LAYER

#include "../../logic/feature/Layer.h"

#include <ArduinoJson.h> // Include the appropriate header for ArduinoJson (assuming it's used).
#include <string>

namespace config {
namespace file {
class Layer {
protected:
  static void LoadLedColor(const JsonArray &config, int *color);
  static void LoadKeys(const JsonArray &config,
                       logic::feature::StringMap &keys);
  // static void LoadCombos(const JsonObject &config, logic::feature::StringMap
  // &combos);

public:
  static void Load(const std::string name, const JsonObject &config);
};
} // namespace file
} // namespace config
#endif