#include "Timeline.h"

#include "../../utils/Debug.hpp"
#include "../../utils/Memory.h"
#include "../ObjectManager.h"
#include "../Timer.h"
#include "../feature/Key.h"
#include "Universe.h"

namespace logic {
namespace quantum {
Timeline::Timeline(const std::string &i_name)
    : name(i_name), pruned(false), parent(nullptr), complexity(1) {
  // DEBUG_INFO("[CREATE %d] logic::quantum::Timeline::Timeline %s", this,
  //            i_name.c_str());
  logic::ObjectManager::Register("logic::quantum::Timeline");
}

Timeline::~Timeline() {
  // DEBUG_INFO("[DELETE @%d] logic::quantum::Timeline", this);
  ObjectManager::Unregister("logic::quantum::Timeline");
};

Timeline::Ptr Timeline::New(const std::string &name) {
  return Ptr(new Timeline(name));
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

void Timeline::add_layer(logic::feature::Layer::Ptr layer) {
  DEBUG_INFO("logic::quantum::Timeline::add_layer");
  active_layers.push_back(layer);

  for (const auto &pair : layer->get_keys()) {
    const std::string &switch_uid = pair.first;
    const std::vector<std::string> &definitions = pair.second;
    std::string pressed_event = switch_uid + std::string(".pressed");
    layer_events[pressed_event] = std::vector<ActionFuncPtr>();
    for (auto definition : definitions) {
      const logic::KeyFunc &action = logic::feature::Key::Get(definition);
      layer_events[pressed_event].push_back(
          NewActionFunc([action, switch_uid](Timeline &timeline) {
            action(timeline, switch_uid);
          }));
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
  feature::Layer::Ptr merged_layer = active_layers.back();
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
    feature::Layer::Ptr current_layer = active_layers.back();
    active_layers.pop_back();
    add_layer(current_layer);
    current_layer->activate(*this);
  }
}

void Timeline::set_release_action(const std::string event_id,
                                  const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::set_release_action %s: %s",
             name.c_str(), event_id.c_str());
  if (children.size() > 0) {
    for (auto child : children) {
      child->set_release_action(event_id, function);
    }
    return;
  }
  auto item = release_events.find(event_id);
  if (item == release_events.end()) {
    release_events[event_id] = std::vector<ActionFuncPtr>();
    item = release_events.find(event_id);
  }
  item->second.push_back(function);
}

void Timeline::add_commit_action(const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::add_commit_action %s", name.c_str());
  this->commit_actions.push_back(function);
}

void Timeline::add_timer(const std::string timer, int delay_ms) {
  DEBUG_INFO("logic::quantum::Timeline::add_Timer %d: %s ", this, name.c_str());
  timers.push_back(logic::Timer::Start(timer, delay_ms, this));
}

void Timeline::process_event(const std::string &event_id) {
  if (pruned) {
    return;
  }
  DEBUG_INFO("");
  Serial.print("logic::quantum::Timeline::process_event ");
  Serial.println(event_id.c_str());
  if (children.size() > 0) {
    // DEBUG_INFO("Timeline Children");
    for (auto child : children) {
      // DEBUG_INFO("Timeline Child");
      child->process_event(event_id);
    }
    // DEBUG_INFO("Children done");
    return;
  }

  // Handle release events first
  auto item = release_events.find(event_id);
  if (item != release_events.end()) {
    DEBUG_INFO("Process Release");
    for (auto action : item->second) {
      DEBUG_INFO("**********");
      // DEBUG_INFO("Timeline: running lambda %d", action.get());
      (*action)(*this);
    }
    release_events.erase(item);
    return;
  }

  // Select which the map of events to use
  std::map<std::string, std::vector<ActionFuncPtr>> *valid_events;
  if (combo_events.size() > 0) {
    DEBUG_INFO("Process Combo");
    valid_events = &combo_events;
  } else {
    DEBUG_INFO("Process Layer");
    valid_events = &layer_events;
  }

  // Activate the event or end timeline
  item = valid_events->find(event_id);
  if (item != valid_events->end()) {
    // DEBUG_INFO("Timeline: running lambdas");
    for (auto action : item->second) {
      // DEBUG_INFO("Timeline: running lambda %d", action.get());
      DEBUG_INFO("**********");
      (*action)(*this);
    }
    // DEBUG_INFO("Timeline: lambdas done");
  } else {
    DEBUG_INFO("Timeline '%s' ended because of unknown event '%s'",
               name.c_str(), event_id.c_str());
    // for (const auto pair : *valid_events) {
    //   DEBUG_INFO("  - %s", pair.first.c_str());
    // }
    prune();
  }
}
void Timeline::add_combo_event(const std::string event_id,
                               const ActionFuncPtr function) {
  DEBUG_INFO("logic::quantum::Timeline::add_combo_event %s %s (%d)",
             name.c_str(), event_id.c_str(), function.get());
  combo_events[event_id] = std::vector<ActionFuncPtr>();
  combo_events[event_id].push_back(function);
}

void Timeline::process_combo_event(
    const std::string &event_id, const std::string &combo_id,
    const std::vector<std::string> &switches_uid) {
  if (pruned) {
    return;
  }
  DEBUG_INFO(
      "logic::quantum::Timeline::process_combo_event '%s' (%d events left)",
      name.c_str(), combo_events.size());
  combo_events.erase(event_id);
  if (combo_events.size() == 0) {
    // Prune any sibling timeline with a complexity less than the current
    // timeline
    // Warning: Sequences must be processed first to override Chords
    if (parent != nullptr) {
      // DEBUG_INFO("Siblings: %d", parent->children.size());
      for (auto sibling = parent->children.rbegin();
           sibling != parent->children.rend() &&
           (*sibling)->complexity <= complexity;
           ++sibling) {
        // DEBUG_INFO("Sibling %s (%d)", (*sibling)->name.c_str(),
        //            (*sibling)->complexity);
        if (&(**sibling) != this) {
          (*sibling)->prune();
        }
      }
      // Ignore the timer
      // The timer is ignored instead of being stopped in case the timer Event
      // happens in the same loop.
      set_release_action(combo_id + ".timer", ActionFuncNoOp);
    }

    const std::string release_combo_id = combo_id + ".released";
    for (auto switch_uid : switches_uid) {
      std::string release_event_id = "switch." + switch_uid + ".released";
      ActionFuncPtr release_action(
          NewActionFunc([release_combo_id, release_event_id, switch_uid,
                         switches_uid, this](Timeline &timeline) {
            // Execute release actions
            auto actions = release_events.find(release_combo_id);
            if (actions != release_events.end()) {
              timeline.process_event(release_combo_id);
            } else {
              DEBUG_INFO("Event not found %s", release_combo_id.c_str());
            }
          }));
      set_release_action(release_event_id, release_action);
    }
  }
}

Timeline::Ptr Timeline::split(const std::string &id) {
  DEBUG_INFO("logic::quantum::Timeline::split %s + %s", name.c_str(),
             id.c_str());
  const std::string new_name = name + "|" + id;
  Ptr child(New(new_name));
  // utils::Memory::PrintMemoryUsage();

  child->parent = this;
  children.push_back(child);

  child->layer_events =
      std::map<std::string, std::vector<ActionFuncPtr>>(layer_events);
  child->release_events =
      std::map<std::string, std::vector<ActionFuncPtr>>(release_events);
  // child->combo_events; an active combo cannot be split.
  child->active_layers = std::list<logic::feature::Layer::Ptr>(active_layers);

  DEBUG_INFO("Split Timeline has %d layer events and %d release events",
             child->layer_events.size(), child->release_events.size());
  return child;
}

void Timeline::execute() {
  DEBUG_INFO("logic::quantum::Timeline::execute");
  for (auto action : commit_actions) {
    DEBUG_INFO("execute action");
    DEBUG_INFO("**********");
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

  // The timeline is resolved, move to the next node.
  Ptr child = *children.begin();
  DEBUG_INFO("Stepping to the next Timeline: %s (@%d)", child->name.c_str(),
             child);
  child->parent = nullptr;
  child->name = child->name.substr(child->name.find_last_of('|') + 1);
  Universe::StartTimeline(child);
  child->resolve();
  logic::ObjectManager::Print();
  utils::Memory::PrintMemoryUsage();
  DEBUG_INFO("");
}

void Timeline::prune() {
  DEBUG_INFO("logic::quantum::Timeline::prune %d: '%s'", this, name.c_str());
  pruned = true;

  // Terminate children.
  children.clear();
}

bool Timeline::clean() {
  DEBUG_INFO("logic::quantum::Timeline::clean %d: '%s' (%d)", this,
             name.c_str(), complexity);

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
    pruned = (children.empty() && parent != nullptr);
  }

  return pruned;
}
} // namespace quantum
} // namespace logic