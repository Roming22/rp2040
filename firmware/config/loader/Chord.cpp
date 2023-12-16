#include "Chord.h"

#include "../../utils/Debug.hpp"
#include "Key.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

namespace config {
namespace loader {
void Chord::Load(const std::string switch_uids, const std::string &definition) {
  DEBUG_INFO("config::loader::Chord::Load: %s", switch_uids.c_str());

  std::map<std::string, std::string> sequences;
  for (auto sequence : GenerateSequences(switch_uids, definition)) {
    DEBUG_VERBOSE("Expanding '%s: %s' to '%s: %s'", switch_uids.c_str(),
                  definition.c_str(), sequence.c_str(), definition.c_str());
    sequences[sequence] = definition;
  }
  for (auto pair : sequences) {
    DEBUG_INFO("Chord %s: %s", pair.first.c_str(), pair.second.c_str());
  }
  config::loader::Key::Load(definition);
}

std::vector<std::string>
Chord::GenerateSequences(const std::string &switch_uids,
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
  std::vector<std::string> permutations;
  std::sort(switches.begin(), switches.end());
  do {
    std::stringstream result;
    result << switches[0]; // Add the first element without a separator
    for (size_t i = 1; i < switches.size(); ++i) {
      result << "+" << switches[i]; // Add the separator and the next element
    }
    permutations.push_back(result.str());
  } while (std::next_permutation(switches.begin(), switches.end()));

  // Transform permutations to sequence definitions
  std::vector<std::string> sequences;
  for (auto permutation : permutations) {
    switches.clear();
    std::istringstream stream(permutation);
    while (std::getline(stream, token, '+')) {
      switches.push_back(token);
    }

    std::string sequence = definition;
    for (auto it = switches.rbegin(); it != switches.rend(); ++it) {
      sequence = "CH(" + *it + "," + sequence + ")";
    }
    sequences.push_back(sequence);
  }
  return sequences;
}

} // namespace loader
} // namespace config