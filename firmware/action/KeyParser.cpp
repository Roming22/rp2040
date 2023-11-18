#include "KeyParser.h"

#include "../utils/Debug.hpp"
#include "Combo.h"
#include "Keycode.h"
#include "Layer.h"
#include "MultiTap.h"
#include "TapHold.h"

#include <functional>

namespace action {
std::function<void(logic::quantum::Timeline &)>
KeyParser::Load(const std::string &switch_uid, const std::string &definition) {
  DEBUG_VERBOSE("KeyParse::Load");
  const auto [name, args] = KeyParser::ParseDefinition(definition);
  DEBUG_VERBOSE("Func name: %s", name.c_str());
  if (loader.count(name) == 0) {
    DEBUG_ERROR("Unknown Key function: %s", name.c_str());

    DEBUG_VERBOSE("Known functions: ");
    for (const auto &pair : loader) {
      DEBUG_VERBOSE("  - %s", pair.first.c_str());
    }
  }
  return loader[name](switch_uid, args);
}

std::tuple<std::string, std::vector<std::string>>
KeyParser::ParseDefinition(const std::string &keycode) {
  DEBUG_VERBOSE("KeyParse::ParseDefinition");
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
         std::function<std::function<void(logic::quantum::Timeline &)>(
             const std::string &, const std::vector<std::string> &)>>
    KeyParser::loader = {
        {"KEYCODE", &Keycode::LoadDefinition},
        // {"LY_MO", &Layer::LoadMomentaryDefinition},
        // {"LY_TO", &Layer::LoadToggleDefinition},
        // {"MT", &MultiTap::LoadDefinition},
        // {"TH_HD", &TapHold::LoadHoldDefinition},
        // {"TH_NO", &TapHold::LoadNoneDefinition},
        // {"TH_TP", &TapHold::LoadTapDefinition},
        // {"SQ", &Combo::LoadDefinition},
};
} // namespace action