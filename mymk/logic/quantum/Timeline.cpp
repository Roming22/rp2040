#include "Timeline.h"
#include "../../feature/key/KeyParser.h"
#include "../../utils/Debug.hpp"
#include <functional>

Timeline::Timeline(const std::string &i_history, Timeline *i_parent)
    : history(i_history), parent(i_parent), children(), is_determined(false),
      next_timeline(nullptr), active_layers(), commit_actions() {
  DEBUG_VERBOSE("Timeline::Timeline");
  DEBUG_INFO("New Timeline: %s (@%d)", i_history.c_str(), this);
  if (current == nullptr) {
    current = this;
  }
  if (parent != nullptr) {
    parent->mark_determined();
    parent->children.insert(this);
    possible_events = parent->possible_events;
    // std::vector<Layer> tmp_layer_queue = parent->active_layers;
    // while (!tmp_layer_queue.empty()) {
    //   active_layers.push_back(tmp_layer_queue.front());
    //   tmp_layer_queue.pop();
    // }
  }
  DEBUG_INFO("New Timeline has %d events", possible_events.size());
}

void Timeline::add_event_function(std::string event_id,
                                  std::function<void(Timeline &)> function) {
  DEBUG_INFO("Timeline::add_event_function %s: %s", history.c_str(),
             event_id.c_str());
  this->possible_events[event_id] = function;
}

void Timeline::remove_event_function(std::string event_id) {
  DEBUG_INFO("Timeline::remove_event_function %s: %s", history.c_str(),
             event_id.c_str());
  this->possible_events.erase(this->possible_events.find(event_id));
}

void Timeline::add_commit_action(std::function<void(Timeline &)> function) {
  DEBUG_INFO("Timeline::add_commit_action %s", history.c_str());
  this->commit_actions.push(function);
}

void Timeline::process_event(std::string &event_id) {
  DEBUG_VERBOSE("Timeline::process_event");
  current = this;
  DEBUG_INFO("Timeline %s: processing the '%s' event", history.c_str(),
             event_id.c_str());
  DEBUG_INFO("Timeline events: %d", possible_events.size());
  if (possible_events.count(event_id) > 0) {
    DEBUG_INFO("Timeline maps the event");
    std::function<void(Timeline &)> &function = possible_events[event_id];
    function(*this);
  } else {
    DEBUG_INFO("Timeline '%s' @%d ignores the event", history.c_str(), this);

    if (parent != nullptr) {
      parent->children.erase(parent->children.find(this));
      delete this;
    }
  }
}

Timeline &Timeline::split(const std::string &id) {
  DEBUG_VERBOSE("Timeline::split");
  const std::string new_history = history + "|" + id;
  Timeline &new_timeline = *new Timeline(new_history, this);
  DEBUG_INFO("Split Timeline has %d events", possible_events.size());
  return new_timeline;
}

void Timeline::mark_determined() {
  DEBUG_VERBOSE("Timeline::mark_determined");
  is_determined = true;
  if (parent != nullptr) {
    parent->next_timeline = this;
  }
}

void Timeline::execute() {
  DEBUG_VERBOSE("Timeline::execute");
  while (!commit_actions.empty()) {
    std::function<void(Timeline &)> &action = commit_actions.front();
    commit_actions.pop();
    action(*this);
  }
}

Timeline *Timeline::resolve() {
  DEBUG_VERBOSE("Timeline::resolve");
  DEBUG_INFO("Start timeline: %s (%s, %d)", history.c_str(),
             is_determined ? "true" : "false", children.size());
  DEBUG_INFO("Start Timeline has %d events", possible_events.size());

  if (!is_determined) {
    return this;
  }
  execute();
  if (children.size() != 1) {
    return this;
  }

  // The current timeline has no purpose anymore,
  // move to the next node.
  Timeline &child = **children.begin();
  child.parent = nullptr;
  child.history = child.history.substr(child.history.find_last_of('|') + 1);
  delete this;
  current = child.resolve();
  return current;
}

Timeline *Timeline::current = nullptr;
