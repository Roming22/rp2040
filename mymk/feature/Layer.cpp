#include "Layer.h"
#include "../hardware/led/Pixels.h"
#include "../logic/quantum/Timeline.h"
#include "../utils/Debug.hpp"
#include "key/KeyParser.h"

#include <cstring>
#include <functional>
#include <map>
#include <string>

Layer::Layer(const std::string &i_name, const int *i_color,
             std::map<std::string, std::function<void(Timeline &)>> &i_keys,
             std::map<std::string, std::function<void(Timeline &)>> &i_combos)
    : name(i_name), color(nullptr), keys(i_keys), combos(i_combos) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}

void Layer::LoadConfig(const std::string name, const JsonObject &config) {
  DEBUG_INFO("Layer::LoadConfig: %s", name.c_str());

  int color[] = {-1, -1, -1, 0};
  LoadLedColor(config["leds"]["color"].as<JsonArray>(), color);

  std::map<std::string, std::function<void(Timeline &)>> keys;
  LoadKeys(config["keys"].as<JsonArray>(), keys);

  std::map<std::string, std::function<void(Timeline &)>> combos;
  LoadCombos(config["combos"], combos);

  layers[name] = new Layer(name, color, keys, combos);

  DEBUG_DEBUG("Layer configuration loaded");
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

void Layer::LoadKeys(
    const JsonArray &config,
    std::map<std::string, std::function<void(Timeline &)>> &keys) {
  DEBUG_VERBOSE("Layer::LoadKeys: %d keys", config.size());
  for (JsonVariant v : config) {
    std::string switch_uid = "switch." + std::to_string(keys.size() + 1);
    std::string definition = v.as<std::string>();
    std::string pressed_uid = switch_uid + std::string(".pressed");
    std::string released_uid = switch_uid + std::string(".released");
    keys[pressed_uid] = KeyParser::Load(switch_uid, definition);
    DEBUG_DEBUG("%s: %s", switch_uid.c_str(), v.as<std::string>().c_str());
  }
}

void Layer::LoadCombos(
    const JsonObject &config,
    std::map<std::string, std::function<void(Timeline &)>> &combos) {
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

std::function<void(Timeline &)>
Layer::LoadDefinition(const std::string &switch_uid,
                      const std::vector<std::string> &definition,
                      const bool is_toggle) {
  DEBUG_VERBOSE("Layer::LoadDefinition");
  const std::string layer_name = definition[0];
  return [switch_uid, layer_name, is_toggle](Timeline &timeline) {
    Get(layer_name).on_press(timeline, switch_uid, is_toggle);
  };
}

std::function<void(Timeline &)>
Layer::LoadMomentaryDefinition(const std::string &switch_uid,
                               const std::vector<std::string> &definition) {
  DEBUG_INFO("Layer::LoadMomentaryDefinition");
  return LoadDefinition(switch_uid, definition, false);
}

std::function<void(Timeline &)>
Layer::LoadToggleDefinition(const std::string &switch_uid,
                            const std::vector<std::string> &definition) {
  DEBUG_INFO("Layer::LoadToggleDefinition");
  return LoadDefinition(switch_uid, definition, true);
}

Layer Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return *layers[name];
}

void Layer::on_press(Timeline &timeline, const std::string &switch_uid,
                     const bool is_toggle) {
  const std::string timeline_id =
      "layer." + name + (std::string(is_toggle ? ".toggle" : ".momentary"));
  Timeline &new_timeline = timeline.split(timeline_id);
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On press actions
  DEBUG_INFO("%s", press_event.c_str());
  this->add_to_timeline(new_timeline);
  DEBUG_INFO("New Timeline %s events after load: %d (@%d)",
             new_timeline.history.c_str(), new_timeline.possible_events.size(),
             &new_timeline);
  new_timeline.mark_determined();

  // On commit actions
  new_timeline.add_commit_action(
      [this](Timeline &timeline) { this->on_commit(timeline); });

  // On release configuration
  new_timeline.add_event_function(
      release_event, [this, release_event, is_toggle](Timeline &timeline) {
        this->on_release(timeline, release_event, is_toggle);
      });
}

void Layer::on_commit(Timeline &timeline) const {
  // Set LEDs
  DEBUG_INFO("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  Pixels::Set(0, color[0], color[1], color[2]);
}

void Layer::on_release(Timeline &timeline, const std::string &release_event,
                       const bool is_toggle) {
  DEBUG_INFO("%s", release_event.c_str());
  if (!is_toggle) {
    // Remove layer on release
  }
  timeline.remove_event_function(release_event);
}

void Layer::add_to_timeline(Timeline &timeline) {
  DEBUG_INFO("Timeline::add_to_timeline");
  for (const auto &pair : keys) {
    const std::string pressed_id = pair.first;
    const std::function<void(Timeline &)> function = pair.second;
    timeline.possible_events[pressed_id] = function;
  }
  DEBUG_INFO("Timeline events %s after load: %d (@%d)",
             timeline.history.c_str(), timeline.possible_events.size(),
             &timeline);
}

std::map<const std::string, const Layer *> Layer::layers;
