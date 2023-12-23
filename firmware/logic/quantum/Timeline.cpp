#include "Timeline.h"

#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "../feature/Key.h"
#include "Universe.h"
#include <cstddef>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <vector>

namespace logic {
namespace quantum {
Timeline::Timeline(const std::string &i_name)
    : name(i_name), pruned(false), parent(nullptr), complexity(1) {
  DEBUG_INFO("[CREATE %d] logic::quantum::Timeline::Timeline %s", this,
             i_name.c_str());
}

Timeline::Timeline(const Timeline &copy)
    : name(copy.name), pruned(copy.pruned), parent(copy.parent),
      complexity(copy.complexity), children(copy.children),
      active_layers(copy.active_layers), layer_events(copy.layer_events),
      combo_events(copy.combo_events), commit_actions(copy.commit_actions),
      end_actions(copy.end_actions) {
  DEBUG_INFO("[CREATE %d] logic::quantum::Timeline::Timeline %s", this,
             name.c_str());
}

void Timeline::set_complexity(const int i_complexity) {
  complexity = i_complexity;
  if (parent != nullptr) {
    auto old_index = parent->children.end();
    auto new_index = parent->children.end();
    for (auto iter = parent->children.begin(); iter != parent->children.end();
         ++iter) {
      if (old_index == parent->children.end() && &(**iter) == this) {
        old_index = iter;
        if (new_index != parent->children.end()) {
          iter = parent->children.end();
        }
      }
      if (new_index == parent->children.end() &&
          this->complexity > (*iter)->complexity) {
        new_index = iter;
        if (old_index != parent->children.end()) {
          iter = parent->children.end();
        }
      }
    }
    parent->children.insert(new_index, *old_index);
    parent->children.erase(old_index);
  }
}
void Timeline::set_name(const std::string i_name) { name = i_name; }
std::list<Timeline::Ptr> &Timeline::get_children() { return children; }

void Timeline::add_layer(logic::feature::LayerPtr layer) {
  DEBUG_INFO("logic::quantum::Timeline::add_layer");
  active_layers.push_back(layer);

  for (const auto &pair : layer->get_keys()) {
    const std::string &switch_uid = pair.first;
    const std::vector<std::string> &definitions = pair.second;
    std::string pressed_event = switch_uid + std::string(".pressed");
    layer_events[pressed_event] = std::vector<ActionFuncPtr>();
    for (auto definition : definitions) {
      const logic::KeyFunc &action = logic::feature::Key::Get(definition);
      layer_events[pressed_event].push_back(ActionFuncPtr(
          new ActionFunc([action, switch_uid](Timeline &timeline) {
            action(timeline, switch_uid);
          })));
    }
  }
  DEBUG_INFO("logic::quantum::Timeline layers %s after load: %d", name.c_str(),
             active_layers.size());
  DEBUG_INFO("logic::quantum::Timeline events %s after load: %d (@%d)",
             name.c_str(), layer_events.size(), this);
}

void Timeline::merge_layers() {
  DEBUG_INFO("logic::quantum::Timeline::merge_layers");
  // The only the last layer is kept.
  feature::LayerPtr merged_layer = active_layers.back();
  name = "layer." + merged_layer->name + ".toggle";
  active_layers.clear();
  active_layers.push_back(merged_layer);
  DEBUG_INFO("logic::quantum::Timeline layers %s after merge: %d", name.c_str(),
             active_layers.size());
  DEBUG_INFO("logic::quantum::Timeline events %s after merge: %d (@%d)",
             name.c_str(), layer_events.size(), this);
}

void Timeline::remove_layer(const logic::feature::Layer &layer) {
  // Ignore the removal if it is the only active layer.
  if (active_layers.size() == 1) {
    return;
  }
  DEBUG_INFO("logic::quantum::Timeline::remove_layer");
  DEBUG_INFO("%d Layers left", active_layers.size());
  bool layer_deleted = false;
  for (auto it : active_layers) {
    if (&(*it) == &layer) {
      active_layers.remove(it);
      DEBUG_INFO("Layer found, %d layers left", active_layers.size());
      layer_deleted = true;
      break;
    }
  }
  if (layer_deleted) {
    feature::LayerPtr current_layer = active_layers.back();
    active_layers.pop_back();
    add_layer(current_layer);
    current_layer->activate(*this);
  }
}

void Timeline::set_event_action(const std::string event_id,
                                const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::set_event_function %s: %s",
             name.c_str(), event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->set_event_action(event_id, function);
    }
    return;
  }
  layer_events[event_id] = std::vector<ActionFuncPtr>();
  layer_events[event_id].push_back(function);
}

void Timeline::remove_event_action(const std::string event_id) {
  DEBUG_INFO("logic::quantum::Timeline::remove_event_function %s: %s",
             name.c_str(), event_id.c_str());
  auto item = layer_events.find(event_id);
  if (item != layer_events.end()) {
    layer_events.erase(item);
  }
}

void Timeline::clear_events_action() {
  DEBUG_INFO("logic::quantum::Timeline::clear_event_function %s", name.c_str());
  layer_events.clear();
}

void Timeline::add_commit_action(const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::add_commit_action %s", name.c_str());
  this->commit_actions.push_back(function);
}

void Timeline::add_timer(const std::string timer, int delay_ms) {
  timers.push_back(logic::Timer::Start(timer, delay_ms, this));
}

void Timeline::stop_timers() {
  for (auto timer : timers) {
    timer->stop();
  }
}
// void Timeline::add_end_action(const ActionFuncPtr function) {
//   DEBUG_INFO("logic::quantum::Timeline::add_end_action %s", name.c_str());
//   this->end_actions.push_back(function);
// }

void Timeline::process_event(const std::string &event_id) {
  if (pruned) {
    return;
  }
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Timeline::process_event %s: %s", name.c_str(),
             event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->process_event(event_id);
    }
    return;
  }

  // Select which the map of events to use
  std::map<std::string, std::vector<ActionFuncPtr>> *valid_events;
  if (combo_events.size() > 0) {
    valid_events = &combo_events;
  } else {
    valid_events = &layer_events;
  }

  // Activate the event or end timeline
  auto item = valid_events->find(event_id);
  if (item != valid_events->end()) {
    DEBUG_VERBOSE("Timeline: running lambdas");
    for (auto action : item->second) {
      (*action)(*this);
    }
    DEBUG_VERBOSE("Timeline: lambda done");
  } else {
    if (valid_events->find("ignore_unknown_events") == valid_events->end()) {
      DEBUG_INFO("Timeline '%s' ended because of unknown event '%s'",
                 name.c_str(), event_id.c_str());
      for (const auto pair : *valid_events) {
        DEBUG_DEBUG("  - %s", pair.first.c_str());
      }
      prune();
    } else {
      DEBUG_INFO("Unknown event '%s' ignored", event_id.c_str());
    }
  }
}
void Timeline::add_combo_event(const std::string event_id,
                               const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::add_combo_event %s %s", name.c_str(),
             event_id.c_str());
  combo_events[event_id] = std::vector<ActionFuncPtr>();
  combo_events[event_id].push_back(function);
}

void Timeline::process_combo_event(const std::string &event_id,
                                   const std::string &chord_id,
                                   const std::vector<std::string> &switches_uid,
                                   const std::string &timer_id) {
  if (pruned) {
    return;
  }
  DEBUG_INFO("logic::quantum::Timeline::process_combo_event '%s'",
             name.c_str());
  combo_events.erase(event_id);
  if (combo_events.size() == 0) {
    stop_timers();
    // Prune any sibling timeline with a complexity less than the current
    // timeline
    // Warning: Sequences must be processed first to override Chords
    if (parent != nullptr) {
      std::vector<Ptr> end_list;
      for (auto sibling = parent->children.rbegin();
           sibling != parent->children.rend() &&
           (*sibling)->complexity <= parent->complexity;
           ++sibling) {
        if (&(**sibling) != parent) {
          (*sibling)->prune();
        }
      }
    }

    // Create a release event for each switch belonging to the combo
    for (auto switch_uid : switches_uid) {
      std::string release_event_id = "switch." + switch_uid + ".released";
      ActionFuncPtr release_action(
          new ActionFunc([release_event_id, chord_id, switch_uid, switches_uid,
                          this](Timeline &timeline) {
            // Execute release action
            timeline.process_event(chord_id + ".released");
            // Ignore the release event from any other switch from the combo
            for (auto other_switch_uid : switches_uid) {
              if (switch_uid != other_switch_uid) {
                timeline.set_event_action(
                    "switch." + other_switch_uid + ".released", ActionFuncNoOp);
              }
            }
            timeline.remove_event_action(release_event_id);
          }));
      set_event_action(release_event_id, release_action);
    }
  }
}

Timeline::Ptr Timeline::split(const std::string &id) {
  DEBUG_INFO("*** logic::quantum::Timeline::split %s + %s", name.c_str(),
             id.c_str());
  const std::string new_name = name + "|" + id;
  Ptr child(new Timeline(new_name));
  utils::Memory::PrintMemoryUsage();

  child->parent = this;
  children.push_back(child);

  child->layer_events =
      std::map<std::string, std::vector<ActionFuncPtr>>(layer_events);
  // child->combo_events; an active combo cannot be split.
  child->active_layers = std::list<logic::feature::LayerPtr>(active_layers);

  DEBUG_INFO("*** Split Timeline has %d events", child->layer_events.size());
  return child;
}

void Timeline::execute() {
  DEBUG_INFO("logic::quantum::Timeline::execute");
  for (auto action : commit_actions) {
    DEBUG_INFO("execute action");
    (*action)(*this);
    DEBUG_INFO("action executed");
  }
  // commit_actions.clear();
  while (!commit_actions.empty()) {
    commit_actions.pop_back();
  }
}

void Timeline::resolve() {
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Timeline::resolve: %s (%d)", name.c_str(),
             children.size());

  this->clean();

  if (children.size() > 1) {
    utils::Memory::PrintMemoryUsage();
    DEBUG_INFO("Undetermined");
    return;
  }
  execute();
  if (children.size() != 1) {
    DEBUG_INFO("Timeline is pending (%d children)", children.size());
    utils::Memory::PrintMemoryUsage();
    DEBUG_INFO("");
    return;
  }

  // The current timeline has no purpose anymore,
  // move to the next node.
  Ptr child = *children.begin();
  DEBUG_INFO("Stepping to the next Timeline: %s (@%d)", child->name.c_str(),
             child);
  child->parent = nullptr;
  child->name = child->name.substr(child->name.find_last_of('|') + 1);
  Universe::StartTimeline(child);
  child->resolve();
  utils::Memory::PrintMemoryUsage();
  DEBUG_INFO("");
}

void Timeline::prune() {
  DEBUG_INFO("logic::quantum::Timeline::prune '%s'", name.c_str());
  pruned = true;
  stop_timers();

  // Terminate children.
  children.clear();
}

bool Timeline::clean() {
  DEBUG_INFO("logic::quantum::Timeline::clean '%s'", name.c_str());

  // Propagate to all children
  std::vector<std::list<Ptr>::iterator> to_delete;
  for (auto child = children.begin(); child != children.end(); ++child) {
    if ((*child)->clean()) {
      to_delete.push_back(child);
    }
  }

  if (!to_delete.empty()) {
    for (auto child = to_delete.rbegin(); child != to_delete.rend(); ++child) {
      children.erase(*child);
    }
    pruned = children.empty();
  }

  return pruned;
}
} // namespace quantum
} // namespace logic