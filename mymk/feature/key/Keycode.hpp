#ifndef MYMK_FEATURE_KEY_KEYCODE
#define MYMK_FEATURE_KEY_KEYCODE

#include <string>
#include <vector>

class Timeline;

class Keycode {
public:
  static void LoadDefinition(Timeline *timeline, const std::string &switch_uid,
                             const std::vector<std::string> &definition) {
    DEBUG_VERBOSE("Keycode::Load");
  }
};
#endif