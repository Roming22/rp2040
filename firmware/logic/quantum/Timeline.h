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
  bool pruned;
  Timeline *parent;
  const int complexity;
  std::list<Timeline *> children;
  std::list<logic::feature::LayerPtr> active_layers;

public:
  std::string history;

  std::map<std::string, std::vector<ActionFuncPtr>> layer_events;
  std::map<std::string, std::vector<ActionFuncPtr>> combo_events;
  std::vector<ActionFunc> commit_actions;
  std::vector<ActionFunc> end_actions;

  Timeline(const std::string &i_history, Timeline *i_parent,
           const int complexity);

  std::list<Timeline *> &get_children();

  void add_layer(logic::feature::LayerPtr layer);

  void merge_layers();

  void remove_layer(const logic::feature::Layer &layer);

  void set_event_action(const std::string event_id,
                        const ActionFuncPtr function);

  void remove_event_action(const std::string event_id);

  void clear_events_action();

  void add_commit_action(const ActionFunc function);

  void add_end_action(const ActionFunc function);

  void process_event(const std::string &event_id);

  void add_combo_event(const std::string event_id,
                       const ActionFuncPtr function);

  void process_combo_event(const std::string &event_id,
                           const std::string &chord_id,
                           const std::vector<std::string> &switches_uid,
                           const std::string &timer_id);

  Timeline &split(const std::string &id, const int complexity);

  void execute();

  void resolve();

  void prune();

  bool clean();
};
} // namespace quantum
} // namespace logic
#endif