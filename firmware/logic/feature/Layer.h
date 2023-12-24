#ifndef MYMK_LOGIC_FEATURE_LAYER
#define MYMK_LOGIC_FEATURE_LAYER

#include "../typedef.h"

#include <map>
#include <memory>
#include <string>

namespace logic {
namespace feature {
class Layer;

typedef std::map<std::string, std::vector<std::string>> KeyMap;

class Layer {
public:
  typedef std::shared_ptr<Layer> Ptr;

protected:
  static std::map<const std::string, Layer::Ptr> layers;
  int *color;
  KeyMap keys;

public:
  const std::string name;
  Layer(const std::string &i_name, const int *i_color, KeyMap &i_keys);
  ~Layer();
  bool operator==(const Layer &right) const;

  static void Add(const std::string &name, const int *color, KeyMap &keys);
  static Layer::Ptr Get(const std::string &name);

  const KeyMap &get_keys() const;
  static void OnPress(std::string name, logic::quantum::Timeline &timeline,
                      const std::string &switch_uid, const bool is_toggle);
  void activate(logic::quantum::Timeline &timeline) const;
};
} // namespace feature
} // namespace logic
#endif