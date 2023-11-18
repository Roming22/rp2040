#ifndef MYMK_FEATURE_KEY_KEYCODE
#define MYMK_FEATURE_KEY_KEYCODE

#include "../logic/quantum/Timeline.h"

#include <string>
#include <vector>

namespace action {
class Keycode {
protected:
public:
  static std::function<void(logic::quantum::Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition);

  static void OnPress(logic::quantum::Timeline &timeline,
                      const std::string &switch_uid,
                      const std::string &keycode);

  static void OnCommit(logic::quantum::Timeline &timeline,
                       const std::string &keycode);

  static void OnRelease(logic::quantum::Timeline &timeline,
                        const std::string &switch_uid,
                        const std::string &keycode);
};
} // namespace action
#endif