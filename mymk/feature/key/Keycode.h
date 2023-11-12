#ifndef MYMK_FEATURE_KEY_KEYCODE
#define MYMK_FEATURE_KEY_KEYCODE

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class Keycode {
protected:
public:
  static std::function<void(Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition);

  static void OnPress(Timeline &timeline, const std::string &switch_uid,
                      const std::string &keycode);

  static void OnCommit(Timeline &timeline, const std::string &keycode);

  static void OnRelease(Timeline &timeline, const std::string &switch_uid,
                        const std::string &keycode);
};
#endif