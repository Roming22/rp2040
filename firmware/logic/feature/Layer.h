#ifndef MYMK_LOGIC_FEATURE_LAYER
#define MYMK_LOGIC_FEATURE_LAYER

#include "../typedef.h"

#include <map>
#include <string>

namespace logic {
namespace feature {
typedef std::map<std::string, std::string> StringMap;
class Layer {
protected:
  static std::map<const std::string, const Layer *> layers;
  const std::string name;
  int *color;
  StringMap keys;
  StringMap combos;

public:
  Layer(const std::string &i_name, const int *i_color, StringMap &i_keys,
        StringMap &i_combos);
  static void Add(const std::string &name, const int *color, StringMap &keys,
                  StringMap &combos);
  static Layer Get(const std::string &name);

  const StringMap &get_keys() const;
  void on_press(logic::quantum::Timeline &timeline,
                const std::string &switch_uid, const bool is_toggle);
  void on_commit(logic::quantum::Timeline &timeline) const;
  void on_release(logic::quantum::Timeline &timeline,
                  const std::string &switch_uid, const bool is_toggle);
};
} // namespace feature
} // namespace logic
#endif