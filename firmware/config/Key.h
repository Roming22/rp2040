#ifndef MYMK_CONFIG_KEY
#define MYMK_CONFIG_KEY

#include "KeyFunc.h"

#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace config {

class Key {
protected:
  static std::map<std::string,
                  std::function<KeyFunc(const std::vector<std::string> &)>>
      loader;
  static std::map<std::string, KeyFunc> key_func;

  static std::tuple<std::string, std::vector<std::string>>
  ParseDefinition(const std::string &keycode);

public:
  static void Load(const std::string &definition);
};
} // namespace config
#endif