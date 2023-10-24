#include "Layer.h"
#include "../hardware/led/Pixels.h"
#include "../logic/quantum/Timeline.h"
#include "../utils/Debug.hpp"

#include <cstring>

Layer::Layer(const std::string &i_name, const int *i_color,
             std::map<std::string, std::string> &i_keys,
             std::map<std::string, std::function<void()>> &i_combos)
    : name(i_name), color(nullptr), keys(i_keys), combos(i_combos) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}

void Layer::LoadConfig(const std::string name, const JsonObject &config) {
  DEBUG_INFO("Layer::Load: %s", name.c_str());

  int color[] = {-1, -1, -1, 0};
  LoadLedColor(config["leds"]["color"].as<JsonArray>(), color);

  std::map<std::string, std::string> keys;
  LoadKeys(config["keys"].as<JsonArray>(), keys);

  std::map<std::string, std::function<void()>> combos;
  LoadCombos(config["combos"], combos);

  layers[name] = new Layer(name, color, keys, combos);

  DEBUG_DEBUG("Layer loaded");
}

void Layer::LoadLedColor(const JsonArray &config, int *color) {
  DEBUG_VERBOSE("Layer::LoadLedColor");
  if (config.size() < 3 || config.size() > 4) {
    DEBUG_WARNING("[WARNING] Incorrect led color length: %d", config.size());
    return;
  }
  for (int i = 0; i < config.size(); ++i) {
    color[i] = config[i].as<int>();
  }
}

void Layer::LoadKeys(const JsonArray &config,
                     std::map<std::string, std::string> &keys) {
  DEBUG_VERBOSE("Layer::LoadKeys: %d keys", config.size());
  for (JsonVariant v : config) {
    std::string event_uid = "switch." + std::to_string(keys.size());
    keys[event_uid] = v.as<std::string>();
    DEBUG_DEBUG("%s: %s", event_uid.c_str(), v.as<std::string>().c_str());
  }
}

void Layer::LoadCombos(const JsonObject &config,
                       std::map<std::string, std::function<void()>> &combos) {
  DEBUG_VERBOSE("Layer::LoadCombos");

  DEBUG_DEBUG("Loading chords");
  for (JsonPair kvp : config["chords"].as<JsonObject>()) {
    std::string definition = kvp.key().c_str();
    std::string key = kvp.value().as<std::string>();
    DEBUG_DEBUG("Chord %s: %s", definition.c_str(), key.c_str());
  }
  DEBUG_DEBUG("Loading sequences");
  for (JsonPair kvp : config["sequences"].as<JsonObject>()) {
    std::string definition = kvp.key().c_str();
    std::string key = kvp.value().as<std::string>();
    DEBUG_DEBUG("Sequence: %s: %s", definition.c_str(), key.c_str());
  }
}

const Layer &Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return *layers[name];
}

void Layer::LoadKeyDefinition(Timeline &timeline, const std::string &switch_uid,
                              const std::vector<std::string> &definition,
                              const bool is_toggle) {
  DEBUG_VERBOSE("KeyLayer::LoadDefinition");
  std::string layer_name = definition[0];
  DEBUG_INFO("Loading layer '%s'", layer_name.c_str());

  Layer layer = Layer::Get(layer_name);
  const std::string timeline_id =
      "layer." + layer_name +
      (std::string(is_toggle ? ".toggle" : ".momentary"));
  Timeline &new_timeline = timeline.split(timeline_id);

  for (const auto &pair : layer.keys) {
    const std::string id = pair.first;
    const std::string definition = pair.second;
    new_timeline.possible_events[id + ".pressed"] = definition;
  }

  if (!is_toggle) {
    // TODO: Implement momentary layers
  }
  new_timeline.active_layers.push(layer);
  new_timeline.actions.push([layer]() { layer.set_leds(); });
  // TODO: Add release definition on switch release
  new_timeline.mark_determined();
}

void Layer::LoadMomentaryDefinition(
    Timeline &timeline, const std::string &switch_uid,
    const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("KeyLayer::LoadMomentaryDefinition");
  Layer::LoadKeyDefinition(timeline, switch_uid, definition, false);
}

void Layer::LoadToggleDefinition(Timeline &timeline,
                                 const std::string &switch_uid,
                                 const std::vector<std::string> &definition) {
  DEBUG_INFO("KeyLayer::LoadToggleDefinition");
  Layer::LoadKeyDefinition(timeline, switch_uid, definition, true);
}

void Layer::set_leds() const {
  DEBUG_INFO("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  Pixels::Set(0, color[0], color[1], color[2]);
}

std::map<const std::string, const Layer *> Layer::layers;
