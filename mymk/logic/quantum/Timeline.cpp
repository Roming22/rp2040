#include "Timeline.h"
#include "../../feature/key/KeyParser.h"
#include "../../utils/Debug.hpp"

void Timeline::process_event(std::string &event_id) {
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
  if (!is_determined) {
    return this;
  }
  execute();
  if (children.size() != 1) {
    return this;
  }
  return children[0].resolve();
}
