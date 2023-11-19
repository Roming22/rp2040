#include "Key.h"

// #include "../../action/Combo.h"
#include "../key/Keycode.h"
// #include "../../action/Layer.h"
// #include "../../action/MultiTap.h"
// #include "../../action/TapHold.h"
#include "../../logic/feature/Key.h"
#include "../../utils/Debug.hpp"

namespace config {
namespace file {
void Key::Load(const std::string &definition) {
  DEBUG_VERBOSE("config::Key::Load: %s", definition.c_str());
  if (logic::feature::Key::Has(definition)) {
    return;
  }
  const auto [name, args] = Key::ParseDefinition(definition);
  DEBUG_VERBOSE("Func name: %s", name.c_str());
  if (loader.count(name) == 0) {
    DEBUG_ERROR("Unknown Key function: %s", name.c_str());

    DEBUG_VERBOSE("Known functions: ");
    for (const auto &pair : loader) {
      DEBUG_VERBOSE("  - %s", pair.first.c_str());
    }
  }
  logic::feature::Key::Set(definition, loader[name](args));
}

std::tuple<std::string, std::vector<std::string>>
Key::ParseDefinition(const std::string &keycode) {
  DEBUG_VERBOSE("config::Key::ParseDefinition");
  std::string func_name;
  std::vector<std::string> args;

  // Find the index of the left parenthesis
  size_t left_parenthesis = keycode.find("(");
  if (left_parenthesis == std::string::npos) {
    // No parentheses found, set default values
    func_name = "KEYCODE";
    args.push_back(keycode);
  } else {
    // Parse the string to extract the function name and its arguments
    func_name = keycode.substr(0, left_parenthesis);

    // Extract the arguments part of the string
    std::string args_str = keycode.substr(
        left_parenthesis + 1, keycode.size() - left_parenthesis - 2);

    // Initialize variables for tracking the substrings and open parentheses
    // count
    std::string current_substring;
    int open_parentheses = 0;

    // Loop through each character in the arguments string
    for (char character : args_str) {
      if (character == '(') {
        open_parentheses++;
      } else if (character == ')') {
        open_parentheses--;
      }

      if (character == '|' && open_parentheses == 0) {
        args.push_back(current_substring);
        current_substring.clear();
      } else {
        current_substring += character;
      }
    }

    // Append the last substring to the output list
    args.push_back(current_substring);
  }

  return std::make_tuple(func_name, args);
}

std::map<std::string,
         std::function<logic::KeyFunc(const std::vector<std::string> &)>>
    Key::loader = {
        {"KEYCODE", &key::Keycode::Load},
        // {"LY_MO", &Layer::LoadMomentaryDefinition},
        // {"LY_TO", &Layer::LoadToggleDefinition},
        // {"MT", &MultiTap::LoadDefinition},
        // {"TH_HD", &TapHold::LoadHoldDefinition},
        // {"TH_NO", &TapHold::LoadNoneDefinition},
        // {"TH_TP", &TapHold::LoadTapDefinition},
        // {"SQ", &Combo::LoadDefinition},
};
} // namespace file
} // namespace config