#ifndef MYMK_LOGIC_QUANTUM_UNIVERSE
#define MYMK_LOGIC_QUANTUM_UNIVERSE

// The Universe manages Timelines, creating branches and collapsing
// them based on the events that are triggered during its lifetime.

#include "../../feature/Layer.hpp"
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
  static void Init(std::string layer_name) {
    DEBUG_INFO("Universe::Init %s", layer_name.c_str());
    start_state = new Timeline("start", nullptr);
    // The default layer will activate 'start' event.
    start_state->possible_events["start"] = "LY_TO(" + layer_name + ")";
  }

  static void Start() {
    DEBUG_INFO("Universe::Start");
    active_state = start_state;
    // TODO Send a 'start' event.
  }
};
Timeline *Universe::start_state = nullptr;
Timeline *Universe::active_state = nullptr;
#endif