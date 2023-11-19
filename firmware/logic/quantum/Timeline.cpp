#include "Timeline.h"
#include "../../utils/Debug.hpp"
#include "../feature/Key.h"
#include "Universe.h"

namespace logic {
namespace quantum {
Timeline::Timeline(const std::string &i_history, Timeline *i_parent)
    : history(i_history), parent(i_parent), children(), is_determined(false),
      next_timeline(nullptr), active_layers(), commit_actions() {
  DEBUG_VERBOSE("logic::quantum::Timeline::Timeline");
  DEBUG_INFO("New Timeline: %s (@%d)", i_history.c_str(), this);
  if (parent != nullptr) {
    parent->mark_determined();
    parent->children.push_back(this);
    possible_events = parent->possible_events;
    // TODO: Copy active_layers?
  }
  DEBUG_VERBOSE("New Timeline has %d events", possible_events.size());
}

void Timeline::End(Timeline &timeline) { timeline.end(); }

void Timeline::add_layer(logic::feature::Layer &layer) {
  DEBUG_INFO("logic::quantum::Timeline::add_layer");
  active_layers.push_back(layer);

  for (const auto &pair : layer.get_keys()) {
    const std::string &switch_uid = pair.first;
    const std::string &definition = pair.second;
    const logic::KeyFunc &action = logic::feature::Key::Get(definition);
    std::string pressed_event = switch_uid + std::string(".pressed");
    possible_events[pressed_event] = [action, switch_uid](Timeline &timeline) {
      action(timeline, switch_uid);
    };
  };
  DEBUG_INFO("logic::quantum::Timeline events %s after load: %d (@%d)",
             history.c_str(), possible_events.size(), this);
}

void Timeline::add_event_action(const std::string event_id,
                                const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_event_function %s: %s",
             history.c_str(), event_id.c_str());
  this->possible_events[event_id] = function;
}

void Timeline::remove_event_action(const std::string event_id) {
  DEBUG_INFO("logic::quantum::Timeline::remove_event_function %s: %s",
             history.c_str(), event_id.c_str());
  if (this->possible_events.count(event_id) > 0) {
    this->possible_events.erase(this->possible_events.find(event_id));
  }
}

void Timeline::add_commit_action(const ActionFunc function) {
  DEBUG_INFO("logic::quantum::Timeline::add_commit_action %s", history.c_str());
  this->commit_actions.push_back(function);
}

void Timeline::process_event(const std::string &event_id) {
  DEBUG_VERBOSE("Timeline::process_event %s", history.c_str());
  if (children.size() > 0) {
    for (int i = children.size() - 1; i >= 0; --i) {
      DEBUG_VERBOSE("Child #%d", i + 1);
      children[i]->process_event(event_id);
    }
    return;
  }
  DEBUG_INFO("Timeline %s: processing the '%s' event", history.c_str(),
             event_id.c_str());
  if (possible_events.count(event_id) > 0) {
    DEBUG_VERBOSE("Timeline: running lambda");
    ActionFunc &function = possible_events[event_id];
    function(*this);
    DEBUG_VERBOSE("Timeline: lambda done");
  } else {
    if (event_id != "interrupt") {
      end();
    }
  }
}

Timeline &Timeline::split(const std::string &id) {
  DEBUG_VERBOSE("logic::quantum::Timeline::split");
  const std::string new_history = history + "|" + id;
  Timeline &new_timeline = *new Timeline(new_history, this);
  DEBUG_VERBOSE("Split Timeline has %d events", possible_events.size());
  return new_timeline;
}

void Timeline::mark_determined() {
  DEBUG_INFO("logic::quantum::Timeline::mark_determined %s", history.c_str());
  is_determined = true;
  if (parent != nullptr) {
    parent->next_timeline = this;
  }
}

void Timeline::execute() {
  DEBUG_VERBOSE("logic::quantum::Timeline::execute");
  for (int i = 0; i < commit_actions.size(); ++i) {
    ActionFunc &action = commit_actions[i];
    action(*this);
  }
  commit_actions.clear();
}

void Timeline::resolve() {
  DEBUG_VERBOSE("Timeline::resolve");
  DEBUG_INFO("Resolving timeline: %s (%s, %d)", history.c_str(),
             is_determined ? "true" : "false", children.size());

  if (!is_determined) {
    utils::Memory::PrintMemoryUsage();
    DEBUG_INFO("");
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
  DEBUG_INFO("Moving onto the next Timeline");
  Timeline &child = *children[0];
  DEBUG_INFO("Moving onto the next Timeline: %s (@%d)", child.history.c_str(),
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
  DEBUG_INFO("Timeline::end '%s'", history.c_str());
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