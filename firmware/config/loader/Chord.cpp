#include "Chord.h"

#include "../../utils/Debug.hpp"
#include "Key.h"

#include <iostream>
#include <sstream>

namespace config {
namespace loader {
void Chord::Load(const std::string switch_uids, const std::string &definition,
                 logic::feature::KeyMap &keys) {
  DEBUG_INFO("config::loader::Chord::Load: %s", switch_uids.c_str());

  for (auto pair : GenerateConfiguration(switch_uids, definition)) {
    const std::string &keycode = std::get<1>(pair);
    DEBUG_INFO("  - '%s: %s'", std::get<0>(pair).c_str(), keycode.c_str());
    keys[std::get<0>(pair)].push_back(keycode);
    // config::loader::Key::Load(keycode);
  }
}

std::vector<std::tuple<std::string, std::string>>
Chord::GenerateConfiguration(const std::string &switch_uids,
                             const std::string &definition) {
  DEBUG_VERBOSE("config::loader::Layer::GenerateSequences");

  // Tokenize switch_uids
  std::vector<std::string> switches;
  std::istringstream stream(switch_uids);
  std::string token;
  while (std::getline(stream, token, '+')) {
    switches.push_back(token);
  }

  // Generate all permutations
  std::vector<std::tuple<std::string, std::string>> configuration;
  std::sort(switches.begin(), switches.end());
  for (auto _switch : switches) {
    std::ostringstream chord;
    chord << "CH(";
    for (auto it : switches) {
      if (_switch != it) {
        chord << it << ",";
      }
    }
    chord << definition << ")";
    configuration.push_back(std::make_tuple(_switch, chord.str()));
  }
  return configuration;
}

} // namespace loader
} // namespace config