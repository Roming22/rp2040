#ifndef MYMK_LOGIC_FEATURE_CHORD
#define MYMK_LOGIC_FEATURE_CHORD

#include "../quantum/Timeline.h"

#include <string>
#include <vector>

namespace logic {
namespace feature {
class Chord {
public:
  static int delay_ms;
  static void OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition);
};
} // namespace feature
} // namespace logic
#endif