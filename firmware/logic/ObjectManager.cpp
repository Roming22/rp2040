#include "ObjectManager.h"

#include "../utils/Debug.hpp"

namespace logic {
std::map<std::string, int> ObjectManager::counter =
    std::map<std::string, int>();

void ObjectManager::Register(const std::string &key) {
  auto item = counter.find(key);
  int value = 1;
  if (item != counter.end()) {
    value = item->second + 1;
  }
  counter[key] = value;
}
void ObjectManager::Unregister(const std::string &key) {
  auto item = counter.find(key);
  if (item == counter.end()) {
    DEBUG_ERROR("[ERROR] %s is not registered", key.c_str());
    Print();
  }
  counter[key] = item->second - 1;
  if (counter[key] < 0) {
    DEBUG_ERROR("[ERROR] Too many %s deleted", key.c_str());
    Print();
  }
}
void ObjectManager::Print() {
  DEBUG_INFO("OBJECT MANAGER:");
  for (auto item : counter) {
    DEBUG_INFO("  * %s = %d", item.first.c_str(), item.second);
  }
}

} // namespace logic