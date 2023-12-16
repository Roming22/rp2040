#include "Timeline.h"

#include "../../utils/Debug.hpp"
#include "../feature/Key.h"
#include "Universe.h"
#include <vector>

namespace logic {
namespace quantum {
Timeline::Timeline(const std::string &i_history, Timeline *i_parent,
                   const int i_complexity)
    : history(i_history), parent(i_parent), complexity(i_complexity),
      children(), active_layers(), commit_actions() {
  DEBUG_VERBOSE("logic::quantum::Timeline::Timeline");
  DEBUG_INFO("New Timeline: %s (@%d)", i_history.c_str(), this);
  if (parent != nullptr) {
    parent->children.push_back(this);
    for (auto kvp : parent->possible_events) {
      possible_events[kvp.first] = std::vector<ActionFunc>();
      for (auto item : kvp.second) {
        possible_events[kvp.first].push_back(item);
      }
    }
    for (auto item : parent->active_layers) {
      active_layers.push_back(item);
    }
  }
  DEBUG_VERBOSE("New Timeline has %d events", possible_events.size());
}

std::vector<Timeline *> &Timeline::get_children() { return children; }

void Timeline::add_layer(logic::feature::LayerPtr layer) {
  DEBUG_INFO("logic::quantum::Timeline::add_layer");
  active_layers.push_back(layer);

  for (const auto &pair : layer->get_keys()) {
    const std::string &switch_uid = pair.first;
    const std::vector<std::string> &definitions = pair.second;
    std::string pressed_event = switch_uid + std::string(".pressed");
    possible_events[pressed_event] = std::vector<ActionFunc>();
    for (auto definition : definitions) {
      const logic::KeyFunc &action = logic::feature::Key::Get(definition);
      possible_events[pressed_event].push_back(
          [action, switch_uid](Timeline &timeline) {
            action(timeline, switch_uid);
          });
    }
  }
  DEBUG_INFO("logic::quantum::Timeline layers %s after load: %d",
             history.c_str(), active_layers.size());
  DEBUG_INFO("logic::quantum::Timeline events %s after load: %d (@%d)",
             history.c_str(), possible_events.size(), this);
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
  DEBUG_INFO("logic::quantum::Timeline::add_event_function %s: %s",
             history.c_str(), event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->set_event_action(event_id, function);
    }
    return;
  }
  possible_events[event_id] = std::vector<ActionFunc>();
  possible_events[event_id].push_back(function);
}

void Timeline::remove_event_action(const std::string event_id) {
  DEBUG_INFO("logic::quantum::Timeline::remove_event_function %s: %s",
             history.c_str(), event_id.c_str());
  auto item = possible_events.find(event_id);
  if (item != possible_events.end()) {
    possible_events.erase(item);
  }
}

void Timeline::clear_events_action() {
  DEBUG_INFO("logic::quantum::Timeline::clear_event_function %s",
             history.c_str());
  possible_events.clear();
}

void Timeline::add_commit_action(const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_commit_action %s", history.c_str());
  this->commit_actions.push_back(function);
}

void Timeline::process_event(const std::string &event_id) {
  DEBUG_INFO("logic::quantum::Timeline::process_event %s: %s", history.c_str(),
             event_id.c_str());
  if (children.size() > 0) {
    for (int it = 0; it < children.size(); ++it) {
      children[it]->process_event(event_id);
    }
    return;
  }
  auto item = possible_events.find(event_id);
  if (item != possible_events.end()) {
    DEBUG_VERBOSE("Timeline: running lambdas");
    for (auto action : item->second) {
      DEBUG_INFO("");
      action(*this);
    }
    DEBUG_VERBOSE("Timeline: lambda done");
  } else {
    if (possible_events.find("ignore_unknown_events") ==
        possible_events.end()) {
      end();
      DEBUG_INFO("Timeline ended");
    } else {
      DEBUG_INFO("Event ignored");
    }
  }
}

Timeline &Timeline::split(const std::string &id, const int complexity) {
  DEBUG_VERBOSE("logic::quantum::Timeline::split");
  const std::string new_history = history + "|" + id;
  Timeline &new_timeline = *new Timeline(new_history, this, complexity);
  DEBUG_VERBOSE("Split Timeline has %d events", possible_events.size());
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

void Timeline::end() {
  DEBUG_INFO("logic::quantum::Timeline::end '%s'", history.c_str());
  if (parent != nullptr) {
    for (auto child = parent->children.begin(); child != parent->children.end();
         ++child) {
      if (*child == this) {
        parent->children.erase(child);
        break;
      }
    }
    delete this;
  }
}
} // namespace quantum
} // namespace logic