#ifndef MYMK_FEATURE_KEY_MULTITAP
#define MYMK_FEATURE_KEY_MULTITAP

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class MultiTap {
public:
  static std::function<void(Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition);

  static void OnPress(Timeline &timeline, const std::string &switch_even,
                      const std::vector<std::string> &definition);

  static void OnCommit(Timeline &timeline,
                       const std::vector<std::string> &definition);

  static void OnRelease(Timeline &timeline, const std::string &release_event,
                        const std::vector<std::string> &definition);
};
#endif