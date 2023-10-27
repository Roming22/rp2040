#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../feature/Layer.h"

#include <functional>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

class Timeline {
protected:
  static Timeline *current;
  bool is_determined;
  Timeline *next_timeline;
  Timeline *parent;
  std::set<Timeline *> children;

public:
  std::string history;

  std::vector<Layer> active_layers;
  std::map<std::string, std::function<void(Timeline &)>> possible_events;
  std::vector<std::function<void(Timeline &)>> commit_actions;

  Timeline(const std::string &i_history, Timeline *i_parent);

  void activate_layer(const std::string &layer_name);

  void add_event_function(std::string event_id,
                          std::function<void(Timeline &)> function);

  void remove_event_function(std::string event_id);

  void add_commit_action(std::function<void(Timeline &)> function);

  void process_event(std::string &event_id);

  Timeline &split(const std::string &id);

  void mark_determined();

  void execute();

  Timeline *resolve();
};
#endif