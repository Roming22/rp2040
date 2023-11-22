#include "Layer.h"
#include "../../logic/feature/Layer.h"
#include "../../logic/typedef.h"
#include "../../utils/Debug.hpp"

namespace config {
namespace key {
logic::KeyFunc Layer::Load(const std::vector<std::string> &definition,
                           const bool is_toggle) {
  DEBUG_VERBOSE("config::key::Layer::Load");
  return [definition, is_toggle](logic::quantum::Timeline &timeline,
                                 const std::string &switch_uid) {
    for (auto layer_name : definition) {
      logic::feature::Layer::OnPress(layer_name, timeline, switch_uid,
                                     is_toggle);
    }
  };
}

logic::KeyFunc
Layer::LoadMomentary(const std::vector<std::string> &definition) {
  DEBUG_INFO("config::key::Layer::LoadMomentary");
  return Load(definition, false);
}

logic::KeyFunc Layer::LoadToggle(const std::vector<std::string> &definition) {
  DEBUG_INFO("config::key::Layer::LoadToggle");
  return Load(definition, true);
}
} // namespace key
} // namespace config