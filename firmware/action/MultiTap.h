#ifndef MYMK_FEATURE_KEY_MULTITAP
#define MYMK_FEATURE_KEY_MULTITAP

#include "../logic/quantum/Timeline.h"

#include <string>
#include <vector>

namespace action {
class MultiTap {
public:
  static std::function<void(logic::quantum::Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition);

  static void OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_even,
                      const std::vector<std::string> &definition);

  static void OnCommit(logic::quantum::Timeline &timeline,
                       const std::vector<std::string> &definition);

  static void OnRelease(logic::quantum::Timeline &timeline,
                        const std::string &release_event,
                        const std::vector<std::string> &definition);
};
} // namespace action
#endif