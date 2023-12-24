#ifndef MYMK_LOGIC_FEATURE_LAYER
#define MYMK_LOGIC_FEATURE_LAYER

#include "../../hardware/led/Pixels.h"
#include "../typedef.h"

#include <map>
#include <memory>
#include <string>

namespace logic {
namespace feature {

typedef std::map<std::string, std::vector<std::string>> KeyMap;

class Layer {
public:
  typedef std::shared_ptr<Layer> Ptr;

protected:
  static std::map<const std::string, Layer::Ptr> layers;
  hardware::led::Pixels::ColorPtr color;
  KeyMap keys;

public:
  const std::string name;
  Layer(const std::string &i_name,
        const hardware::led::Pixels::ColorPtr i_color, KeyMap &i_keys);
  ~Layer();
  bool operator==(const Layer &right) const;

  static Ptr New(const std::string &name,
                 const hardware::led::Pixels::ColorPtr color, KeyMap &keys);
  static void Add(const std::string &name,
                  const hardware::led::Pixels::ColorPtr color, KeyMap &keys);
  static Layer::Ptr Get(const std::string &name);

  const KeyMap &get_keys() const;
  static void OnPress(std::string name, logic::quantum::Timeline &timeline,
                      const std::string &switch_uid, const bool is_toggle);
  void activate(logic::quantum::Timeline &timeline) const;
};
} // namespace feature
} // namespace logic
#endif