#ifndef MYMK_CONFIG_LOADER_CHORD
#define MYMK_CONFIG_LOADER_CHORD

#include <string>
#include <vector>

namespace config {
namespace loader {
class Chord {
protected:
  static std::vector<std::string>
  GenerateSequences(const std::string &switch_uids,
                    const std::string &definition);

public:
  static void Load(const std::string name, const std::string &definition);
};
} // namespace loader
} // namespace config
#endif