#ifndef MYMK_FEATURE_KEY_MULTITAP
#define MYMK_FEATURE_KEY_MULTITAP

#include <string>
#include <vector>

class Timeline;

class MultiTap {
public:
  static void LoadDefinition(Timeline *timeline, const std::string &switch_uid,
                             const std::vector<std::string> &definition) {
    DEBUG_VERBOSE("MultiTap::Load");
  }
};
#endif