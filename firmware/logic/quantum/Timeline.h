#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../logic/feature/Layer.h"
#include "../typedef.h"

#include <functional>
#include <list>
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
  std::list<Timeline *> children;
  std::list<logic::feature::LayerPtr> active_layers;

public:
  std::string history;

  std::map<std::string, ActionFunc> possible_events;
  std::vector<ActionFunc> commit_actions;

  Timeline(const std::string &i_history, Timeline *i_parent);

  static void End(Timeline &timeline);

  void add_layer(logic::feature::LayerPtr layer);

  void merge_layers();

  void remove_layer(const logic::feature::Layer &layer);

  void add_event_action(const std::string event_id, const ActionFunc function);

  void remove_event_action(const std::string event_id);

  void add_commit_action(const ActionFunc function);

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