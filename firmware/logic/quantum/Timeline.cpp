#include "Timeline.h"

#include "../../utils/Debug.hpp"
#include "../Timer.h"
#include "../feature/Key.h"
#include "Universe.h"

namespace logic {
namespace quantum {
Timeline::Timeline(const std::string &i_history, Timeline *i_parent,
                   const int i_complexity)
    : history(i_history), pruned(false), parent(i_parent),
      complexity(i_complexity), children(), active_layers(), commit_actions(),
      combo_events() {
  DEBUG_VERBOSE("logic::quantum::Timeline::Timeline");
  DEBUG_INFO("New Timeline: %s (@%d)", i_history.c_str(), this);
  if (parent != nullptr) {
    // Ordering is very important when dealing with combos
    // Sequences must be before Chords to override them
    auto child = parent->children.begin();
    for (; child != parent->children.end(); ++child) {
      if (complexity > (*child)->complexity) {
        break;
      }
    }
    parent->children.insert(child, this);
    for (auto kvp : parent->layer_events) {
      layer_events[kvp.first] = std::vector<ActionFunc>();
      for (auto item : kvp.second) {
        layer_events[kvp.first].push_back(item);
      }
    }
    // The combo_events does not need to copied as a Timeline with an
    // active combo cannot be split.
    for (auto item : parent->active_layers) {
      active_layers.push_back(item);
    }
  }
  DEBUG_INFO("New Timeline has %d events", layer_events.size());
}

std::list<Timeline *> &Timeline::get_children() { return children; }

void Timeline::add_layer(logic::feature::LayerPtr layer) {
  DEBUG_INFO("logic::quantum::Timeline::add_layer");
  active_layers.push_back(layer);

  for (const auto &pair : layer->get_keys()) {
    const std::string &switch_uid = pair.first;
    const std::vector<std::string> &definitions = pair.second;
    std::string pressed_event = switch_uid + std::string(".pressed");
    layer_events[pressed_event] = std::vector<ActionFunc>();
    for (auto definition : definitions) {
      const logic::KeyFunc &action = logic::feature::Key::Get(definition);
      layer_events[pressed_event].push_back(
          [action, switch_uid](Timeline &timeline) {
            action(timeline, switch_uid);
          });
    }
  }
  DEBUG_INFO("logic::quantum::Timeline layers %s after load: %d",
             history.c_str(), active_layers.size());
  DEBUG_INFO("logic::quantum::Timeline events %s after load: %d (@%d)",
             history.c_str(), layer_events.size(), this);
}

void Timeline::merge_layers() {
  DEBUG_INFO("logic::quantum::Timeline::merge_layers");
  feature::LayerPtr current_layer = active_layers.back();
  active_layers.clear();
  feature::LayerPtr merged_layer = feature::Layer::Get(current_layer->name);
  active_layers.push_back(merged_layer);
}

void Timeline::remove_layer(const logic::feature::Layer &layer) {
  DEBUG_INFO("logic::quantum::Timeline::remove_layer");
  DEBUG_INFO("%d Layers left", active_layers.size());
  bool layer_deleted = false;
  for (auto it : active_layers) {
    if (*it == layer) {
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
    current_layer->on_commit(*this);
  }
}

void Timeline::set_event_action(const std::string event_id,
                                const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::set_event_function %s: %s",
             history.c_str(), event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->set_event_action(event_id, function);
    }
    return;
  }
  layer_events[event_id] = std::vector<ActionFunc>();
  layer_events[event_id].push_back(function);
}

void Timeline::remove_event_action(const std::string event_id) {
  DEBUG_INFO("logic::quantum::Timeline::remove_event_function %s: %s",
             history.c_str(), event_id.c_str());
  auto item = layer_events.find(event_id);
  if (item != layer_events.end()) {
    layer_events.erase(item);
  }
}

void Timeline::clear_events_action() {
  DEBUG_INFO("logic::quantum::Timeline::clear_event_function %s",
             history.c_str());
  layer_events.clear();
}

void Timeline::add_commit_action(const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_commit_action %s", history.c_str());
  this->commit_actions.push_back(function);
}

void Timeline::add_end_action(const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_end_action %s", history.c_str());
  this->end_actions.push_back(function);
}

void Timeline::process_event(const std::string &event_id) {
  if (pruned) {
    return;
  }
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Timeline::process_event %s: %s", history.c_str(),
             event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->process_event(event_id);
    }
    return;
  }

  // Select which the map of events to use
  std::map<std::string, std::vector<ActionFunc>> *valid_events;
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
      DEBUG_INFO("");
      action(*this);
    }
    DEBUG_VERBOSE("Timeline: lambda done");
  } else {
    if (valid_events->find("ignore_unknown_events") == valid_events->end()) {
      DEBUG_INFO("Timeline '%s' ended because of unknown event '%s'",
                 history.c_str(), event_id.c_str());
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
                               const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_combo_event %s %s", history.c_str(),
             event_id.c_str());
  combo_events[event_id] = std::vector<ActionFunc>();
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
             history.c_str());
  combo_events.erase(event_id);
  if (combo_events.size() == 0) {
    Timer::Stop(timer_id);
    // Prune any sibling timeline with a complexity less than the current
    // timeline
    // Warning: Sequences must be processed first to override Chords
    if (parent != nullptr) {
      std::vector<Timeline *> end_list;
      for (auto sibling = parent->parent->children.rbegin();
           sibling != parent->parent->children.rend() &&
           (*sibling)->complexity <= parent->complexity;
           ++sibling) {
        if (*sibling != parent) {
          (*sibling)->prune();
        }
      }
    }

    // Create a release event for each switch belonging to the combo
    for (auto switch_uid : switches_uid) {
      std::string release_event_id = "switch." + switch_uid + ".released";
      set_event_action(release_event_id, [release_event_id, chord_id,
                                          switch_uid, switches_uid,
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
      });
    }
  }
}

Timeline &Timeline::split(const std::string &id, const int complexity) {
  DEBUG_VERBOSE("logic::quantum::Timeline::split");
  const std::string new_history = history + "|" + id;
  Timeline &new_timeline = *new Timeline(new_history, this, complexity);
  DEBUG_VERBOSE("Split Timeline has %d events", layer_events.size());
  return new_timeline;
}

void Timeline::execute() {
  DEBUG_INFO("logic::quantum::Timeline::execute");
  for (auto action : commit_actions) {
    DEBUG_INFO("execute action");
    action(*this);
  }
  commit_actions.clear();
}

void Timeline::resolve() {
  DEBUG_INFO("");
  DEBUG_INFO("logic::quantum::Timeline::resolve: %s (%d)", history.c_str(),
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
  Timeline &child = **children.begin();
  DEBUG_INFO("Stepping to the next Timeline: %s (@%d)", child.history.c_str(),
             &child);
  child.parent = nullptr;
  child.history = child.history.substr(child.history.find_last_of('|') + 1);
  delete this;
  Universe::StartTimeline(child);
  child.resolve();
  utils::Memory::PrintMemoryUsage();
  DEBUG_INFO("");
}

void Timeline::prune() {
  DEBUG_INFO("logic::quantum::Timeline::prune '%s'", history.c_str());
  pruned = true;

  // Propagate timeline termination downwards.
  for (auto child : children) {
    child->prune();
  }
}

bool Timeline::clean() {
  DEBUG_INFO("logic::quantum::Timeline::clean '%s'", history.c_str());
  // Propagate to all children
  for (auto child = children.begin(); child != children.end();) {
    if ((*child)->clean()) {
      child = children.erase(child);
      pruned = children.empty();
    } else {
      child++;
    }
  }

  if (pruned) {
    delete this;
    return true;
  }
  return false;
}
} // namespace quantum
} // namespace logic