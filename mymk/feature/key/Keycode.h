#ifndef MYMK_FEATURE_KEY_KEYCODE
#define MYMK_FEATURE_KEY_KEYCODE

#include "../../logic/quantum/Timeline.h"

#include <string>
#include <vector>

class Keycode {
public:
  static std::function<void()>
  LoadDefinition(const std::string &switch_uid,
                 const std::vector<std::string> &definition);

  static void OnPress(const std::string &switch_uid,
                      const std::string &keycode);

  static void OnCommit(const std::string &switch_uid,
                       const std::string &keycode);

  static void OnRelease(const std::string &switch_uid,
                        const std::string &keycode);
};
#endif