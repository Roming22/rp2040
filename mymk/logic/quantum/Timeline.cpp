#include "Timeline.h"
#include "../../feature/key/KeyParser.h"
#include "../../utils/Debug.hpp"

Timeline::Timeline(const std::string &i_history, Timeline *i_parent)
    : history(i_history), parent(i_parent), children(), is_determined(false),
      next_timeline(nullptr), active_layers(), actions() {
  DEBUG_VERBOSE("Timeline::Timeline");
  if (parent != nullptr) {
    parent->mark_determined();
    parent->children.insert(this);
    possible_events = parent->possible_events;
    std::queue<Layer> tmp_layer_queue = parent->active_layers;
    while (!tmp_layer_queue.empty()) {
      active_layers.push(tmp_layer_queue.front());
      tmp_layer_queue.pop();
    }
  }
}

void Timeline::process_event(std::string &event_id) {
  DEBUG_VERBOSE("Timeline::process_event");
  DEBUG_INFO("Timeline %s: processing the '%s' event", history.c_str(),
             event_id.c_str());
  if (possible_events.count(event_id) > 0) {
    std::string &definition = possible_events[event_id];
    DEBUG_INFO("Timeline maps the event to '%s'", definition.c_str());
    std::string switch_uid = event_id.substr(0, event_id.find_last_of("."));
    KeyParser::Load(*this, switch_uid, definition);
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
  while (!actions.empty()) {
    std::function<void()> &action = actions.front();
    actions.pop();
    action();
  }
}

Timeline *Timeline::resolve() {
  DEBUG_VERBOSE("Timeline::resolve");
  DEBUG_INFO("Resolving %s (%s, %d)", history.c_str(),
             is_determined ? "true" : "false", children.size());

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
  return child.resolve();
}
