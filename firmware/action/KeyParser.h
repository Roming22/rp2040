#ifndef MYMK_FEATURE_KEY_KEYPARSER
#define MYMK_FEATURE_KEY_KEYPARSER

#include "../logic/quantum/Timeline.h"

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace action {
class KeyParser {
protected:
  static std::map<std::string,
                  std::function<std::function<void(logic::quantum::Timeline &)>(
                      const std::string &, const std::vector<std::string> &)>>
      loader;

public:
  static std::function<void(logic::quantum::Timeline &)>
  Load(const std::string &switch_uid, const std::string &definition);

  static std::tuple<std::string, std::vector<std::string>>
  ParseDefinition(const std::string &keycode);
};
} // namespace action
#endif