#ifndef MYMK_CONFIG_LAYER
#define MYMK_CONFIG_LAYER

#include <string>
#include <vector>

namespace config {
namespace key {
class Layer {
  logic::KeyFunc Load(const std::vector<std::string> &definition,
                      const bool is_toggle);
  logic::KeyFunc LoadMomentary(const std::vector<std::string> &definition);
  logic::KeyFunc LoadToggle(const std::vector<std::string> &definition);
};
} // namespace key
} // namespace config
#endif