#ifndef MYMK_CONFIG_KEY_KEYCODE
#define MYMK_CONFIG_KEY_KEYCODE

#include "../../logic/typedef.h"

namespace config {
namespace key {
class Keycode {
protected:
public:
  static logic::KeyFunc Load(const std::vector<std::string> &definition);
};
} // namespace key
} // namespace config
#endif