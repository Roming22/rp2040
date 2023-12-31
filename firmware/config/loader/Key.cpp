#include "Key.h"

// #include "../key/MultiTap.h"
#include "../../logic/feature/Key.h"
#include "../../utils/Debug.hpp"
#include "../key/Chord.h"
#include "../key/Keycode.h"
#include "../key/Layer.h"
#include "../key/TapHold.h"

namespace config {
namespace loader {
void Key::Load(const std::string &definition) {
  DEBUG_VERBOSE("config::loader::Key::Load: %s", definition.c_str());
  if (logic::feature::Key::Has(definition)) {
    return;
  }
  const auto [name, args] = Key::ParseDefinition(definition);
  DEBUG_VERBOSE("Func name: %s", name.c_str());
  if (loader.count(name) == 0) {
    DEBUG_ERROR("Unknown Key function '%s' in '%s'. Args: '%s'", name.c_str(),
                definition.c_str());
    DEBUG_ERROR("Args:");
    for (auto arg : args) {
      DEBUG_ERROR("  - '%s'", arg.c_str());
    }

    DEBUG_VERBOSE("Known functions: ");
    for (const auto &pair : loader) {
      DEBUG_VERBOSE("  - %s", pair.first.c_str());
    }
  }
  logic::feature::Key::Set(definition, loader[name](args));
}

std::tuple<std::string, std::vector<std::string>>
Key::ParseDefinition(const std::string &keycode) {
  DEBUG_VERBOSE("config::loader::Key::ParseDefinition");
  std::string func_name;
  std::vector<std::string> args;

  // Find the index of the left parenthesis
  size_t left_parenthesis = keycode.find("(");
  if (left_parenthesis == std::string::npos || keycode == "(") {
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
        {"CH", &config::key::Chord::Load},
        {"KEYCODE", &config::key::Keycode::Load},
        {"LY_MO", &config::key::Layer::LoadMomentary},
        {"LY_TO", &config::key::Layer::LoadToggle},
        // {"MT", &MultiTap::Load},
        {"TH", &config::key::TapHold::Load},
};
} // namespace loader
} // namespace config