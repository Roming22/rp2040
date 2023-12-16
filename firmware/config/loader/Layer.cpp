#include "Layer.h"

#include "../../hardware/led/Pixels.h"
#include "../../logic/feature/Layer.h"
#include "../../logic/typedef.h"
#include "../../utils/Debug.hpp"
#include "Chord.h"
#include "Key.h"
#include <string>
#include <vector>

namespace config {
namespace loader {
void Layer::Load(const std::string name, const JsonObject &config) {
  DEBUG_INFO("config::loader::Layer::Load: %s", name.c_str());

  int color[] = {-1, -1, -1, 0};
  LoadLedColor(config["leds"]["color"].as<JsonArray>(), color);

  logic::feature::KeyMap keys;
  LoadKeys(config["keys"].as<JsonArray>(), keys);
  LoadCombos(config["combos"], keys);

  logic::feature::Layer::Add(name, color, keys);

  DEBUG_DEBUG("Layer configuration loaded");
}

void Layer::LoadLedColor(const JsonArray &config, int *color) {
  DEBUG_VERBOSE("config::loader::Layer::LoadLedColor");
  if (config.size() < 3 || config.size() > 4) {
    DEBUG_WARNING("[WARNING] Incorrect led color length: %d", config.size());
    return;
  }
  for (int i = 0; i < config.size(); ++i) {
    color[i] = config[i].as<int>();
  }
}

void Layer::LoadKeys(const JsonArray &config, logic::feature::KeyMap &keys) {
  DEBUG_VERBOSE("config::loader::Layer::LoadKeys: %d keys", config.size());
  for (JsonVariant v : config) {
    std::string switch_uid = "switch." + std::to_string(keys.size() + 1);
    std::string definition = v.as<std::string>();
    keys[switch_uid] = std::vector<std::string>();
    keys[switch_uid].push_back(definition);
    config::loader::Key::Load(definition);
    DEBUG_DEBUG("%s: %s", switch_uid.c_str(), v.as<std::string>().c_str());
  }
}

void Layer::LoadCombos(const JsonObject &config, logic::feature::KeyMap &keys) {
  DEBUG_VERBOSE("config::loader::Layer::LoadCombos");

  DEBUG_DEBUG("Loading chords");
  for (JsonPair kvp : config["chords"].as<JsonObject>()) {
    std::string switch_uids = kvp.key().c_str();
    std::string definition = kvp.value().as<std::string>();
    config::loader::Chord::Load(switch_uids, definition, keys);
  }
}

} // namespace loader
} // namespace config