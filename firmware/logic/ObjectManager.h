#ifndef MYMK_LOGIC_OBJECTMANAGER
#define MYMK_LOGIC_OBJECTMANAGER

#include <map>
#include <string>

namespace logic {
class ObjectManager {
protected:
  static std::map<std::string, int> counter;

public:
  static void Register(const std::string &);
  static void Unregister(const std::string &);
  static void Print();
};
} // namespace logic
#endif