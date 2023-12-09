#ifndef MYMK_CONFIG_KEY_TAPHOLD
#define MYMK_CONFIG_KEY_TAPHOLD

#include "../../logic/typedef.h"

#include <string>
#include <vector>

namespace config {
namespace key {
class TapHold {
public:
  static logic::KeyFunc Load(const std::vector<std::string> &definition);
};
} // namespace key
} // namespace config
#endif