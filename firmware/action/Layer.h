#ifndef MYMK_FEATURE_LAYER
#define MYMK_FEATURE_LAYER

#include <ArduinoJson.h> // Include the appropriate header for ArduinoJson (assuming it's used).
#include <functional>
#include <map>
#include <string>

namespace logic {
namespace quantum {
class Timeline;
}
} // namespace logic

namespace action {
class Layer {
private:
  static std::map<const std::string, const Layer *> layers;
  const std::string name;
  int *color;
  std::map<std::string, std::function<void(logic::quantum::Timeline &)>> keys;
  std::map<std::string, std::function<void(logic::quantum::Timeline &)>> combos;

  static void LoadLedColor(const JsonArray &config, int *color);

  static void LoadKeys(
      const JsonArray &config,
      std::map<std::string, std::function<void(logic::quantum::Timeline &)>>
          &keys);

  static void LoadCombos(
      const JsonObject &config,
      std::map<std::string, std::function<void(logic::quantum::Timeline &)>>
          &combos);

public:
  Layer(const std::string &i_name, const int *i_color,
        std::map<std::string, std::function<void(logic::quantum::Timeline &)>>
            &i_keys,
        std::map<std::string, std::function<void(logic::quantum::Timeline &)>>
            &i_combos);

  static void LoadConfig(const std::string name, const JsonObject &config);

  static std::function<void(logic::quantum::Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition,
                 const bool is_toggle);

  static std::function<void(logic::quantum::Timeline &)>
  LoadMomentaryDefinition(const std::string &switch_uid,
                          const std::vector<std::string> &definition);

  static std::function<void(logic::quantum::Timeline &)>
  LoadToggleDefinition(const std::string &switch_uid,
                       const std::vector<std::string> &definition);

  static Layer Get(const std::string &name);

  void on_press(logic::quantum::Timeline &timeline,
                const std::string &switch_uid, const bool is_toggle);

  void on_commit(logic::quantum::Timeline &timeline) const;

  void on_release(logic::quantum::Timeline &timeline,
                  const std::string &switch_uid, const bool is_toggle);

  void add_to_timeline(logic::quantum::Timeline &timeline);
};
} // namespace action
#endif