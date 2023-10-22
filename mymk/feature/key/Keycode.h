#ifndef MYMK_FEATURE_KEY_KEYCODE
#define MYMK_FEATURE_KEY_KEYCODE

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class Keycode {
public:
  static void LoadDefinition(Timeline &timeline, const std::string &switch_uid,
                             const std::vector<std::string> &definition);
};
#endif