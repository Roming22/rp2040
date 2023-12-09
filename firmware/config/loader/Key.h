#ifndef MYMK_CONFIG_LOADER_KEY
#define MYMK_CONFIG_LOADER_KEY

#include "../../logic/typedef.h"

#include <map>
#include <string>

namespace config {
namespace loader {
class Key {
protected:
  static std::map<std::string, std::function<logic::KeyFunc(
                                   const std::vector<std::string> &)>>
      loader;

  static std::tuple<std::string, std::vector<std::string>>
  ParseDefinition(const std::string &keycode);

public:
  static void Load(const std::string &definition);
};
} // namespace loader
} // namespace config
#endif