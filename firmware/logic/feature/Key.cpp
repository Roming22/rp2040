#include "Key.h"

#include "../../utils/Debug.hpp"

namespace logic {
namespace feature {
void Key::Set(const std::string &definition, const logic::KeyFunc &func) {
  key_func[definition] = func;
}

bool Key::Has(const std::string &definition) {
  return (key_func.count(definition) != 0);
}

logic::KeyFunc &Key::Get(const std::string &definition) {
  return key_func[definition];
}

std::map<std::string, logic::KeyFunc> Key::key_func;
} // namespace feature
} // namespace logic