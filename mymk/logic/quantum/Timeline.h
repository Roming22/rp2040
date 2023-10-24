#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../feature/Layer.h"

#include <functional>
#include <map>
#include <queue>
#include <set>
#include <string>

class Timeline {
protected:
  std::string history;
  bool is_determined;
  Timeline *next_timeline;
  Timeline *parent;
  std::set<Timeline *> children;

public:
  std::queue<Layer> active_layers;
  std::map<std::string, std::string> possible_events;
  std::queue<std::function<void()>> actions;

  Timeline(const std::string &i_history, Timeline *i_parent);

  void activate_layer(const std::string &layer_name);

  void process_event(std::string &event_id);

  Timeline &split(const std::string &id);

  void mark_determined();

  void execute();

  Timeline *resolve();
};
#endif