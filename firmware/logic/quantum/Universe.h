#ifndef MYMK_LOGIC_QUANTUM_UNIVERSE
#define MYMK_LOGIC_QUANTUM_UNIVERSE

// The Universe manages Timelines, creating branches and collapsing
// them based on the events that are triggered during its lifetime.

#include "Timeline.h"

#include <string>

namespace logic {
namespace quantum {
class Universe {
protected:
  static Timeline::Ptr
      start_timeline; // Starting state of the universe.
                      // As soon as the next state is determined (i.e. there is
                      // but one next state possible, a.k.a "nexus") the start
                      // state is moved to that next state.

public:
  static void Setup(std::string layer_name);

  static void Tick();

  static void StartTimeline(Timeline::Ptr &timeline);
};
} // namespace quantum
} // namespace logic
#endif