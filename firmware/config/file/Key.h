#ifndef MYMK_CONFIG_FILE_KEY
#define MYMK_CONFIG_FILE_KEY

#include "../../logic/typedef.h"

#include <map>
#include <string>

namespace config {
namespace file {
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
} // namespace file
} // namespace config
#endif