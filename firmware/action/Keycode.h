#ifndef MYMK_ACTION_KEY_KEYCODE
#define MYMK_ACTION_KEY_KEYCODE

#include "../logic/quantum/Timeline.h"

#include <string>

namespace action {
class Keycode {
protected:
public:
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