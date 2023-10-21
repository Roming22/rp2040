#ifndef MYMK_LOGIC_QUANTUM_UNIVERSE
#define MYMK_LOGIC_QUANTUM_UNIVERSE

// The Universe manages Timelines, creating branches and collapsing
// them based on the events that are triggered during its lifetime.

#include "../../feature/Layer.hpp"
#include "../../logic/Events.hpp"
#include "Timeline.hpp"

#include <map>
#include <string>
#include <vector>

class Universe {
protected:
  static Timeline
      *start_state; // Starting state of the universe.
                    // As soon as the next state is determined (i.e. there is
                    // but one next state possible, a.k.a "nexus") the start
                    // state is moved to that next state.
  static Timeline *active_state; // The timeline that is currently being
                                 // processed by the universe

public:
  static void Setup(std::string layer_name) {
    DEBUG_INFO("Universe::Setup %s", layer_name.c_str());
    start_state = new Timeline("start", nullptr);
    // The default layer will activate 'start' event.
    start_state->possible_events["start"] = "LY_TO(" + layer_name + ")";
    Event::Add(std::string("start"));
  }

  static void Tick() {
    DEBUG_VERBOSE("Universe::Tick");

    if (Event::HasEvents()) {
      std::string event_id = Event::Get();
      DEBUG_INFO("Universe: processing the '%s' event", event_id.c_str());
    }
  }
};
Timeline *Universe::start_state = nullptr;
Timeline *Universe::active_state = nullptr;
#endif