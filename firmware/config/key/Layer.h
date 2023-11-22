#ifndef MYMK_CONFIG_KEY_LAYER
#define MYMK_CONFIG_KEY_LAYER

#include "../../logic/typedef.h"

#include <string>
#include <vector>

namespace config {
namespace key {
class Layer {
public:
  static logic::KeyFunc Load(const std::vector<std::string> &definition,
                             const bool is_toggle);
  static logic::KeyFunc
  LoadMomentary(const std::vector<std::string> &definition);
  static logic::KeyFunc LoadToggle(const std::vector<std::string> &definition);
};
} // namespace key
} // namespace config
#endif