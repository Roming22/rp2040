#ifndef MYMK_FEATURE_KEY_KEYPARSER
#define MYMK_FEATURE_KEY_KEYPARSER

#include "../../logic/quantum/Timeline.h"
#include "../Layer.h"
#include "Combo.h"
#include "Keycode.h"
#include "MultiTap.h"

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

class KeyParser {
protected:
  static std::map<std::string,
                  std::function<void(Timeline *, const std::string &,
                                     const std::vector<std::string> &)>>
      loader;

public:
  static void Load(Timeline *timeline, const std::string &switch_uid,
                   const std::string &definition);

  static std::tuple<std::string, std::vector<std::string>>
  ParseDefinition(const std::string &keycode);
};
#endif