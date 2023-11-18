#include "Layer.h"
#include "../config/Key.h"
#include "../hardware/led/Pixels.h"
#include "../utils/Debug.hpp"

#include <cstring>
#include <map>
#include <string>

namespace config {
Layer::Layer(const std::string &i_name, const int *i_color, StringMap &i_keys,
             StringMap &i_combos)
    : name(i_name), color(nullptr), keys(i_keys), combos(i_combos) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}

void Layer::Load(const std::string name, const JsonObject &config) {
  DEBUG_INFO("config::Layer::Load: %s", name.c_str());

  int color[] = {-1, -1, -1, 0};
  LoadLedColor(config["leds"]["color"].as<JsonArray>(), color);

  StringMap keys;
  LoadKeys(config["keys"].as<JsonArray>(), keys);

  StringMap combos;
  // LoadCombos(config["combos"], combos);

  layers[name] = new Layer(name, color, keys, combos);

  DEBUG_DEBUG("Layer configuration loaded");
}

void Layer::LoadLedColor(const JsonArray &config, int *color) {
  DEBUG_VERBOSE("config::Layer::LoadLedColor");
  if (config.size() < 3 || config.size() > 4) {
    DEBUG_WARNING("[WARNING] Incorrect led color length: %d", config.size());
    return;
  }
  for (int i = 0; i < config.size(); ++i) {
    color[i] = config[i].as<int>();
  }
}

void Layer::LoadKeys(const JsonArray &config, StringMap &keys) {
  DEBUG_VERBOSE("config::Layer::LoadKeys: %d keys", config.size());
  for (JsonVariant v : config) {
    std::string switch_uid = "switch." + std::to_string(keys.size() + 1);
    std::string definition = v.as<std::string>();
    std::string pressed_uid = switch_uid + std::string(".pressed");
    keys[pressed_uid] = definition;
    config::Key::Load(definition);
    DEBUG_DEBUG("%s: %s", switch_uid.c_str(), v.as<std::string>().c_str());
  }
}

// void Layer::LoadCombos(const JsonObject &config, StringMap &combos) {
//   DEBUG_VERBOSE("config::Layer::LoadCombos");

//   DEBUG_DEBUG("Loading chords");
//   for (JsonPair kvp : config["chords"].as<JsonObject>()) {
//     std::string definition = kvp.key().c_str();
//     std::string key = kvp.value().as<std::string>();
//     DEBUG_DEBUG("Chord %s: %s", definition.c_str(), key.c_str());
//     config::Key::Load(definition);
//   }
//   DEBUG_DEBUG("Loading sequences");
//   for (JsonPair kvp : config["sequences"].as<JsonObject>()) {
//     std::string definition = kvp.key().c_str();
//     std::string key = kvp.value().as<std::string>();
//     DEBUG_DEBUG("Sequence: %s: %s", definition.c_str(), key.c_str());
//     config::Key::Load(definition);
//   }
// }

Layer Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return *layers[name];
}

std::map<const std::string, const Layer *> Layer::layers;
} // namespace config