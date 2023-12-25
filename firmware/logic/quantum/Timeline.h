#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../logic/Timer.h"
#include "../../logic/feature/Layer.h"
#include "../typedef.h"

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace logic {
namespace quantum {
class Timeline {
public:
  typedef std::shared_ptr<Timeline> Ptr;

protected:
  bool pruned;
  Timeline *parent;
  int complexity;
  std::list<Ptr> children;
  std::list<logic::feature::Layer::Ptr> active_layers;

public:
  std::string name;

  std::map<std::string, std::vector<ActionFuncPtr>> layer_events;
  std::map<std::string, std::vector<ActionFuncPtr>> combo_events;
  std::vector<ActionFuncPtr> commit_actions;
  std::vector<logic::Timer::Ptr> timers;

  Timeline(const std::string &i_name);
  ~Timeline();

  static Ptr New(const std::string &name);

  void set_complexity(const int complexity);
  void set_name(const std::string name);
  std::list<Ptr> &get_children();

  // Maybe those functions should be moved to Layer
  void add_layer(logic::feature::Layer::Ptr layer);
  void merge_layers();
  void remove_layer(const logic::feature::Layer &layer);

  void set_event_action(const std::string event_id,
                        const ActionFuncPtr function);
  void remove_event_action(const std::string event_id);
  void add_commit_action(const ActionFuncPtr function);

  void add_timer(const std::string timer, int delay_ms);

  void process_event(const std::string &event_id);

  // Maybe those functions shoud be move to Combo
  void add_combo_event(const std::string event_id,
                       const ActionFuncPtr function);
  void process_combo_event(const std::string &event_id,
                           const std::string &combo_id,
                           const std::vector<std::string> &switches_uid);

  Ptr split(const std::string &id);

  void execute();
  void resolve();
  void prune();
  bool clean();
};
} // namespace quantum
} // namespace logic
#endif