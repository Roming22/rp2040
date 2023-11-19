#include "Layer.h"

#include <cstring>

namespace logic {
namespace feature {
Layer::Layer(const std::string &i_name, const int *i_color, StringMap &i_keys,
             StringMap &i_combos)
    : name(i_name), color(nullptr), keys(i_keys), combos(i_combos) {
  color = new int[4];
  std::memcpy(color, i_color, sizeof(int) * 4);
}

void Layer::Add(const std::string &name, const int *color, StringMap &keys,
                StringMap &combos) {
  layers[name] = new Layer(name, color, keys, combos);
}

Layer Layer::Get(const std::string &name) {
  if (layers.count(name) == 0) {
    DEBUG_ERROR("Unknown layer: '%s'", name.c_str());
  }
  return *layers[name];
}

const StringMap &Layer::get_keys() const { return keys; }

void Layer::on_press(logic::quantum::Timeline &timeline,
                     const std::string &switch_uid, const bool is_toggle) {
  const std::string timeline_id =
      "layer." + name + (std::string(is_toggle ? ".toggle" : ".momentary"));
  logic::quantum::Timeline &new_timeline = timeline.split(timeline_id);
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");

  // On press actions
  DEBUG_INFO("%s", press_event.c_str());
  new_timeline.add_layer(*this);
  DEBUG_INFO("New Timeline %s events after load: %d (@%d)",
             new_timeline.history.c_str(), new_timeline.possible_events.size(),
             &new_timeline);
  new_timeline.mark_determined();

  // On commit actions
  new_timeline.add_commit_action([this](logic::quantum::Timeline &timeline) {
    this->on_commit(timeline);
  });

  // On release configuration
  new_timeline.add_event_action(
      release_event,
      [this, release_event, is_toggle](logic::quantum::Timeline &timeline) {
        this->on_release(timeline, release_event, is_toggle);
      });
}

void Layer::on_commit(logic::quantum::Timeline &timeline) const {
  // Set LEDs
  DEBUG_INFO("Set pixels to [%d, %d, %d]", color[0], color[1], color[2]);
  if (color[0] < 0) {
    return;
  }
  hardware::led::Pixels::Set(0, color[0], color[1], color[2]);
}

void Layer::on_release(logic::quantum::Timeline &timeline,
                       const std::string &release_event, const bool is_toggle) {
  DEBUG_INFO("%s", release_event.c_str());
  if (!is_toggle) {
    // Remove layer on release
  }
  timeline.remove_event_action(release_event);
}

std::map<const std::string, const Layer *> Layer::layers;
} // namespace feature
} // namespace logic