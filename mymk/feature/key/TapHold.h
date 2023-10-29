#ifndef MYMK_FEATURE_KEY_TAPHOLD
#define MYMK_FEATURE_KEY_TAPHOLD

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class TapHold {
protected:
  enum InterruptMode {
    NONE,
    HOLD,
    TAP,
  };

  static std::function<void(Timeline &)>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition,
                 const InterruptMode interrupt_mode);

public:
  static std::function<void(Timeline &)>
  LoadHoldDefinition(const std::string &switch_uid,
                     const std::vector<std::string> &definition);

  static std::function<void(Timeline &)>
  LoadNoneDefinition(const std::string &switch_uid,
                     const std::vector<std::string> &definition);

  static std::function<void(Timeline &)>
  LoadTapDefinition(const std::string &switch_uid,
                    const std::vector<std::string> &definition);

  static void OnPress(Timeline &timeline, const std::string &switch_even,
                      const std::vector<std::string> &definition);

  static void OnCommit(Timeline &timeline,
                       const std::vector<std::string> &definition);

  static void OnRelease(Timeline &timeline, const std::string &release_event,
                        const std::vector<std::string> &definition);
};
#endif