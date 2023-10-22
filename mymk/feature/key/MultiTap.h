#ifndef MYMK_FEATURE_KEY_MULTITAP
#define MYMK_FEATURE_KEY_MULTITAP

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class MultiTap {
public:
  static void LoadDefinition(Timeline *timeline, const std::string &switch_uid,
                             const std::vector<std::string> &definition);
};
#endif