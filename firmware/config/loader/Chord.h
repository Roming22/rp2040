#ifndef MYMK_CONFIG_LOADER_CHORD
#define MYMK_CONFIG_LOADER_CHORD

#include "../../logic/feature/Layer.h"

#include <string>
#include <vector>

namespace config {
namespace loader {
class Chord {
protected:
  static std::vector<std::tuple<std::string, std::string>>
  GenerateConfiguration(const std::string &switch_uids,
                        const std::string &definition);

public:
  static void Load(const std::string name, const std::string &definition,
                   logic::feature::KeyMap &keys);
};
} // namespace loader
} // namespace config
#endif