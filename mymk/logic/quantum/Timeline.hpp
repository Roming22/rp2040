#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../feature/Layer.hpp"

#include <map>
#include <string>
#include <vector>

class Timeline {
protected:
public:
  std::string history;
  bool is_determined;
  Timeline *next_timeline;
  std::map<std::string, std::string> possible_events;
  std::vector<Layer> active_layers;
  std::vector<std::function<void()>> actions;
  Timeline *parent;
  std::vector<Timeline> children;

  Timeline(std::string i_history, Timeline *i_parent)
      : history(i_history), parent(i_parent) {}

  void activate(std::string layer_name) {
    Layer layer = Layer::Get(layer_name);
    DEBUG_INFO("[%s] Load layer '%s'", history.c_str(), layer_name.c_str());
    active_layers.push_back(layer);
    actions.push_back([layer]() { layer.set_leds(); });
  }
};
#endif