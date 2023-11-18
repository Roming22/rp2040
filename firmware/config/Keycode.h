#ifndef MYMK_CONFIG_KEY_KEYCODE
#define MYMK_CONFIG_KEY_KEYCODE

#include "KeyFunc.h"

namespace config {
class Keycode {
protected:
public:
  static KeyFunc Load(const std::vector<std::string> &definition);
};
} // namespace config
#endif