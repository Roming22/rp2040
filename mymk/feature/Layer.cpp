#include "Layer.h"
#include "../hardware/led/Pixels.h"
#include "../logic/quantum/Timeline.h"
#include "../utils/Debug.hpp"

#include <cstring>
#include <functional>
#include <map>
#include <string>

Layer::Layer(const std::string &i_name, const int *i_color,
             std::map<std::string, std::function<void()>> &i_keys,
             std::map<std::string, std::function<void()>> &i_combos)
    : name(i_name), color(nullptr), keys(i_keys), combos(i_combos) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}

void Layer::LoadConfig(const std::string name, const JsonObject &config) {
  DEBUG_INFO("Layer::Load: %s", name.c_str());

  int color[] = {-1, -1, -1, 0};
  LoadLedColor(config["leds"]["color"].as<JsonArray>(), color);

  std::map<std::string, std::function<void()>> keys;
  LoadKeys(config["keys"].as<JsonArray>(), keys);

  std::map<std::string, std::function<void()>> combos;
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

void Layer::LoadKeys(const JsonArray &config,
                     std::map<std::string, std::function<void()>> &keys) {
  DEBUG_VERBOSE("Layer::LoadKeys: %d keys", config.size());
  for (JsonVariant v : config) {
    std::string event_uid = "switch." + std::to_string(keys.size() + 1);
    std::string definition = v.as<std::string>();
    std::string pressed_uid = event_uid + std::string(".pressed");
    std::string released_uid = event_uid + std::string(".released");
    keys[pressed_uid] = [pressed_uid, released_uid, definition]() {
      DEBUG_INFO("%s: %s", pressed_uid.c_str(), definition.c_str());
      Timeline::GetCurrent().possible_events[released_uid] = [released_uid]() {
        DEBUG_INFO("%s", released_uid.c_str());
        Timeline &timeline = Timeline::GetCurrent();
        timeline.possible_events.erase(
            timeline.possible_events.find(released_uid));
      };
    };
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

Layer Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return *layers[name];
}

void Layer::load(const std::string &switch_uid, const bool is_toggle) {
  DEBUG_INFO("KeyLayer::load");
  std::string press_event = switch_uid + std::string(".pressed");
  Timeline::GetCurrent().possible_events[press_event] = [this, switch_uid,
                                                         is_toggle]() {
    this->on_press(switch_uid, is_toggle);
  };
}

void Layer::on_press(const std::string &switch_uid, const bool is_toggle) {
  const std::string timeline_id =
      "layer." + name + (std::string(is_toggle ? ".toggle" : ".momentary"));
  Timeline &new_timeline = Timeline::GetCurrent().split(timeline_id);
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On press actions
  DEBUG_INFO("%s", press_event.c_str());
  this->add_to_timeline();
  new_timeline.mark_determined();

  // On commit actions
  new_timeline.actions.push([this]() { this->set_leds(); });

  // On release configuration
  new_timeline.possible_events[release_event] = [this, release_event,
                                                 is_toggle]() {
    this->on_release(release_event, is_toggle);
  };
}

void Layer::on_release(const std::string &release_event, const bool is_toggle) {
  DEBUG_INFO("%s", release_event.c_str());
  Timeline &timeline = Timeline::GetCurrent();
  timeline.possible_events.erase(timeline.possible_events.find(release_event));
  if (!is_toggle) {
    // Remove layer on release
  }
}

void Layer::add_to_timeline() {
  DEBUG_INFO("Timeline::add_to_timeline");
  Timeline &timeline = Timeline::GetCurrent();
  for (const auto &pair : keys) {
    const std::string pressed_id = pair.first;
    const std::function<void()> function = pair.second;
    timeline.possible_events[pressed_id] = function;
  }
  DEBUG_INFO("Timeline events after load: %d", timeline.possible_events.size());
}

void Layer::set_leds() const {
  DEBUG_INFO("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  Pixels::Set(0, color[0], color[1], color[2]);
}

std::map<const std::string, const Layer *> Layer::layers;
