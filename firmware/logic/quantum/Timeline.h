#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

// #include "../../action/Layer.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace logic {
namespace quantum {
class Timeline {
protected:
  bool is_determined;
  Timeline *next_timeline;
  Timeline *parent;
  std::vector<Timeline *> children;

public:
  std::string history;

  // std::vector<action::Layer> active_layers;
  std::map<std::string, std::function<void(Timeline &)>> possible_events;
  std::vector<std::function<void(Timeline &)>> commit_actions;

  Timeline(const std::string &i_history, Timeline *i_parent);

  static void End(Timeline &timeline);

  void add_event_function(const std::string event_id,
                          const std::function<void(Timeline &)> function);

  void remove_event_function(const std::string event_id);

  void add_commit_action(const std::function<void(Timeline &)> function);

  void process_event(const std::string &event_id);

  Timeline &split(const std::string &id);

  void mark_determined();

  void execute();

  void resolve();

  void end();
};
} // namespace quantum
} // namespace logic
#endif