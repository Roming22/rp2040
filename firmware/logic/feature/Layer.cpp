#include "Layer.h"

#include <cstring>

namespace logic {
namespace feature {
Layer::Layer(const std::string &i_name, const int *i_color, KeyMap &i_keys)
    : name(i_name), color(nullptr), keys(i_keys) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}
bool Layer::operator==(const Layer &right) const {
  DEBUG_INFO("Layer %s (%d_ == %s (%d)", name.c_str(), this, right.name.c_str(),
             &right);
  return (this == &right);
}

void Layer::Add(const std::string &name, const int *color, KeyMap &keys) {
  layers[name] = new Layer(name, color, keys);
}

LayerPtr Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return std::make_shared<Layer>(*layers[name]);
}

const KeyMap &Layer::get_keys() const { return keys; }

void Layer::OnPress(std::string name, logic::quantum::Timeline &timeline,
                    const std::string &switch_uid, const bool is_toggle) {
  DEBUG_INFO("logic::feature::Layer::OnPress %s: %s", switch_uid.c_str(),
             name.c_str());
  const std::string timeline_id =
      "layer." + name + (std::string(is_toggle ? ".toggle" : ".momentary"));
  logic::quantum::Timeline &new_timeline = timeline.split(timeline_id, 1);
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On press actions
  if (name == "" and is_toggle) {
    // Commit the layer and exit
    new_timeline.merge_layers();
    return;
  }
  LayerPtr new_layer = Get(name);
  new_timeline.add_layer(new_layer);
  if (is_toggle) {
    new_timeline.merge_layers();
  }
  DEBUG_INFO("New Timeline %s events after load: %d (@%d)",
             new_timeline.history.c_str(), new_timeline.possible_events.size(),
             &new_timeline);

  // On commit actions
  new_timeline.add_commit_action(
      [new_layer](logic::quantum::Timeline &timeline) {
        new_layer->on_commit(timeline);
      });

  // On release configuration
  new_timeline.set_event_action(
      release_event, [new_layer, release_event,
                      is_toggle](logic::quantum::Timeline &timeline) {
        new_layer->on_release(timeline, release_event, is_toggle);
      });
}

void Layer::on_commit(logic::quantum::Timeline &timeline) const {
  DEBUG_INFO("logic::feature::Layer::on_commit");
  // Set LEDs
  DEBUG_VERBOSE("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  hardware::led::Pixels::Set(0, color[0], color[1], color[2]);
}

void Layer::on_release(logic::quantum::Timeline &timeline,
                       const std::string &release_event, const bool is_toggle) {
  DEBUG_DEBUG("logic::feature::Layer::on_release %s: %s", name.c_str(),
              release_event.c_str());
  if (!is_toggle) {
    // Remove layer on release
    timeline.remove_layer(*this);
  }
  timeline.remove_event_action(release_event);
}

std::map<const std::string, const Layer *> Layer::layers;
} // namespace feature
} // namespace logic