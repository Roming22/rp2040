#ifndef MYMK_FEATURE_KEY_COMBO
#define MYMK_FEATURE_KEY_COMBO

#include "../../logic/quantum/Timeline.h"

class Combo {
public:
  static void LoadDefinition(Timeline *timeline, const std::string &switch_uid,
                             const std::vector<std::string> &definition);
};
#endif
