#ifndef MYMK_CONFIG_KEY_CHORD
#define MYMK_CONFIG_KEY_CHORD

#include "../../logic/typedef.h"

#include <string>
#include <vector>

namespace config {
namespace key {
class Chord {
public:
  static logic::KeyFunc Load(const std::vector<std::string> &definition);
};
} // namespace key
} // namespace config
#endif