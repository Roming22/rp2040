#ifndef MYMK_LOGIC_FEATURE_LAYER
#define MYMK_LOGIC_FEATURE_LAYER

#include "../typedef.h"

#include <map>
#include <memory>
#include <string>

namespace logic {
namespace feature {
class Layer;

typedef std::map<std::string, std::string> StringMap;
typedef std::shared_ptr<Layer> LayerPtr;

class Layer {
protected:
  static std::map<const std::string, const Layer *> layers;
  int *color;
  StringMap keys;
  StringMap combos;

public:
  const std::string name;
  Layer(const std::string &i_name, const int *i_color, StringMap &i_keys,
        StringMap &i_combos);
  bool operator==(const Layer &right) const;

  static void Add(const std::string &name, const int *color, StringMap &keys,
                  StringMap &combos);
  static LayerPtr Get(const std::string &name);

  const StringMap &get_keys() const;
  static void OnPress(std::string name, logic::quantum::Timeline &timeline,
                      const std::string &switch_uid, const bool is_toggle);
  void on_commit(logic::quantum::Timeline &timeline) const;
  void on_release(logic::quantum::Timeline &timeline,
                  const std::string &switch_uid, const bool is_toggle);
};
} // namespace feature
} // namespace logic
#endif