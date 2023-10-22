#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../feature/Layer.h"

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Timeline {
protected:
public:
  std::string history;
  bool is_determined;
  Timeline *next_timeline;
  std::map<std::string, std::string> possible_events;
  std::queue<Layer> active_layers;
  std::queue<std::function<void()>> actions;
  Timeline *parent;
  std::vector<Timeline> children;

  Timeline(std::string i_history, Timeline *i_parent)
      : history(i_history), parent(i_parent) {}

  void activate_layer(const std::string &layer_name);

  void process_event(std::string &event_id);

  void mark_determined();

  void execute();

  Timeline *resolve();
};
#endif