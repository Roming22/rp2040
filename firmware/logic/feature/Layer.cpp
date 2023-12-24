#include "Layer.h"

#include "../../hardware/led/Pixels.h"
#include "../../utils/Debug.hpp"
#include "../quantum/Timeline.h"

#include <cstring>
#include <memory>

namespace logic {
namespace feature {
Layer::Layer(const std::string &i_name, const int *i_color, KeyMap &i_keys)
    : name(i_name), color(nullptr), keys(i_keys) {
  DEBUG_INFO("[CREATE %d] logic::feature::Layer %s", this, name.c_str());
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}
bool Layer::operator==(const Layer &right) const {
  DEBUG_INFO("Layer %s (%d_ == %s (%d)", name.c_str(), this, right.name.c_str(),
             &right);
  return (this == &right);
}

void Layer::Add(const std::string &name, const int *color, KeyMap &keys) {
  layers[name] = Layer::Ptr(new Layer(name, color, keys));
}

Layer::Ptr Layer::Get(const std::string &name) {
  auto item = layers.find(name);
  if (item == layers.end()) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
    exit(1);
  }
  return item->second;
}

const KeyMap &Layer::get_keys() const { return keys; }

void Layer::OnPress(std::string name, logic::quantum::Timeline &timeline,
                    const std::string &switch_uid, const bool is_toggle) {
  DEBUG_INFO("logic::feature::Layer::OnPress %s: %s", switch_uid.c_str(),
             name.c_str());
  const std::string timeline_id =
      "layer." + name + (std::string(is_toggle ? ".toggle" : ".momentary"));
  logic::quantum::Timeline::Ptr new_timeline = timeline.split(timeline_id);
  std::string release_event = switch_uid + std::string(".released");

  // On press actions
  Layer::Ptr new_layer;
  ActionFuncPtr release_action;
  if (name != "") {
    new_layer = Get(name);
    new_timeline->add_layer(new_layer);
    ActionFuncPtr commit_action(
        new ActionFunc([new_layer](logic::quantum::Timeline &timeline) {
          new_layer->activate(timeline);
        }));
    new_timeline->add_commit_action(commit_action);
  }
  if (is_toggle) {
    new_timeline->merge_layers();
    release_action = ActionFuncPtr(
        new ActionFunc([release_event](logic::quantum::Timeline &timeline) {
          timeline.remove_event_action(release_event);
        }));
  } else {
    release_action = ActionFuncPtr(new ActionFunc(
        [new_layer, release_event](logic::quantum::Timeline &timeline) {
          DEBUG_INFO("logic::feature::Layer::deactivate %s: %s",
                     new_layer->name.c_str(), release_event.c_str());
          timeline.remove_layer(*new_layer);
          timeline.remove_event_action(release_event);
        }));
  }
  new_timeline->set_event_action(release_event, release_action);
}

void Layer::activate(logic::quantum::Timeline &timeline) const {
  DEBUG_INFO("logic::feature::Layer::activate");
  // Set LEDs
  DEBUG_VERBOSE("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  hardware::led::Pixels::Set(0, color[0], color[1], color[2]);
}

std::map<const std::string, Layer::Ptr> Layer::layers;
} // namespace feature
} // namespace logic