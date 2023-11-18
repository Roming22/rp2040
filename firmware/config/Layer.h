#ifndef MYMK_CONFIG_LAYER
#define MYMK_CONFIG_LAYER

#include <ArduinoJson.h> // Include the appropriate header for ArduinoJson (assuming it's used).
#include <map>
#include <string>

namespace config {
class Layer {
  typedef std::map<std::string, std::string> StringMap;

private:
  static std::map<const std::string, const Layer *> layers;
  const std::string name;
  int *color;

  StringMap keys;
  StringMap combos;

  static void LoadLedColor(const JsonArray &config, int *color);

  static void LoadKeys(const JsonArray &config, StringMap &keys);

  // static void LoadCombos(const JsonObject &config, StringMap &combos);

public:
  Layer(const std::string &i_name, const int *i_color, StringMap &i_keys,
        StringMap &i_combos);

  static void Load(const std::string name, const JsonObject &config);

  static Layer Get(const std::string &name);
};
} // namespace config
#endif