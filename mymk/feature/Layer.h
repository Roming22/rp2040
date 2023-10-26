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
  std::map<std::string, std::function<void()>> keys;
  std::map<std::string, std::function<void()>> combos;

  static void LoadLedColor(const JsonArray &config, int *color);

  static void LoadKeys(const JsonArray &config,
                       std::map<std::string, std::function<void()>> &keys);

  static void LoadCombos(const JsonObject &config,
                         std::map<std::string, std::function<void()>> &combos);

public:
  Layer(const std::string &i_name, const int *i_color,
        std::map<std::string, std::function<void()>> &i_keys,
        std::map<std::string, std::function<void()>> &i_combos);

  static void LoadConfig(const std::string name, const JsonObject &config);

  static Layer Get(const std::string &name);

  void load(const std::string &switch_uid, const bool is_toggle);

  void on_press(const std::string &switch_event, const bool is_toggle);

  void on_release(const std::string &release_event, const bool is_toggle);

  void add_to_timeline(Timeline &timeline);

  void set_leds() const;
};
#endif