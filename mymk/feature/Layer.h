#ifndef MYMK_FEATURE_LAYER
#define MYMK_FEATURE_LAYER

#include <ArduinoJson.h> // Include the appropriate header for ArduinoJson (assuming it's used).
#include <functional>
#include <map>
#include <string>

class Timeline;

class Layer {
private:
  static std::map<const std::string, const Layer *> layers;
  const std::string name;
  int *color;
  std::map<std::string, std::string> keys;
  std::map<std::string, std::function<void()>> combos;

  static void LoadLedColor(const JsonArray &config, int *color);

  static void LoadKeys(const JsonArray &config,
                       std::map<std::string, std::string> &keys);

  static void LoadCombos(const JsonObject &config,
                         std::map<std::string, std::function<void()>> &combos);

public:
  Layer(const std::string &i_name, const int *i_color,
        std::map<std::string, std::string> &i_keys,
        std::map<std::string, std::function<void()>> &i_combos);

  static void LoadConfig(const std::string name, const JsonObject &config);

  static void LoadKeyDefinition(Timeline &timeline,
                                const std::string &switch_uid,
                                const std::vector<std::string> &definition,
                                const bool is_toggle);

  static void
  LoadMomentaryDefinition(Timeline &timeline, const std::string &switch_uid,
                          const std::vector<std::string> &definition);

  static void LoadToggleDefinition(Timeline &timeline,
                                   const std::string &switch_uid,
                                   const std::vector<std::string> &definition);

  static const Layer &Get(const std::string &name);

  void set_leds() const;
};
#endif