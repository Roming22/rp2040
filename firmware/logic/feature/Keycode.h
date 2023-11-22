#ifndef MYMK_LOGIC_FEATURE_KEYCODE
#define MYMK_LOGIC_FEATURE_KEYCODE

#include "../../logic/quantum/Timeline.h"

#include <string>

namespace logic {
namespace feature {
class Keycode {
protected:
public:
  static void OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::vector<std::string> &definition);

  static void OnCommit(logic::quantum::Timeline &timeline,
                       const std::vector<std::string> &definition);

  static void OnRelease(logic::quantum::Timeline &timeline,
                        const std::string &switch_uid,
                        const std::vector<std::string> &definition);
};
} // namespace feature
} // namespace logic
#endif