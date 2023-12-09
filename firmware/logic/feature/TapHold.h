#ifndef MYMK_LOGIC_FEATURE_TAPHOLD
#define MYMK_LOGIC_FEATURE_TAPHOLD

#include "../quantum/Timeline.h"

#include <string>
#include <vector>

namespace logic {
namespace feature {
class TapHold {
public:
  static int delay_ms;
  static void OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition);
};
} // namespace feature
} // namespace logic
#endif