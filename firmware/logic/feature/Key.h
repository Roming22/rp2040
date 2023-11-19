#ifndef MYMK_LOGIC_FEATURE_KEY
#define MYMK_LOGIC_FEATURE_KEY

#include "../typedef.h"

#include <map>
#include <string>

namespace logic {
namespace feature {
class Key {
protected:
  static std::map<std::string, logic::KeyFunc> key_func;

public:
  static void Set(const std::string &definition, const logic::KeyFunc &func);
  static bool Has(const std::string &definition);
  static logic::KeyFunc &Get(const std::string &definition);
};
} // namespace feature
} // namespace logic
#endif