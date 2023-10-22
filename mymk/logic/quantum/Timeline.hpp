#ifndef MYMK_LOGIC_QUANTUM_TIMELINE
#define MYMK_LOGIC_QUANTUM_TIMELINE

#include "../../feature/key/KeyParser.hpp"

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Layer;

class Timeline {
protected:
public:
  std::string history;
  bool is_determined;
  Timeline *next_timeline;
  std::map<std::string, std::string> possible_events;
  std::queue<Layer> active_layers;
  std::queue<std::function<void()>> actions;
  Timeline *parent;
  std::vector<Timeline> children;

  Timeline(std::string i_history, Timeline *i_parent)
      : history(i_history), parent(i_parent) {}

  void process_event(std::string &event_id) {
    DEBUG_INFO("Timeline %s: processing the '%s' event", history.c_str(),
               event_id.c_str());
    if (possible_events.count(event_id) > 0) {
      std::string &definition = possible_events[event_id];
      DEBUG_INFO("Timeline maps the event to '%s'", definition.c_str());
      std::string switch_uid = event_id.substr(0, event_id.find_last_of("."));
      KeyParser::Load(this, switch_uid, definition);
    } else {
      DEBUG_INFO("Timeline ignores the event");
    }
  }

  void execute() {
    DEBUG_VERBOSE("Timeline::execute");
    while (!actions.empty()) {
      std::function<void()> &action = actions.front();
      actions.pop();
      action();
    }
  }

  Timeline *resolve() {
    DEBUG_VERBOSE("Timeline::resolve");
    if (!is_determined) {
      return this;
    }
    execute();
    if (children.size() != 1) {
      return this;
    }
    return children[0].resolve();
  }
};
#endif