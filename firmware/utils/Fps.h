#ifndef MYMK_UTILS_FPS
#define MYMK_UTILS_FPS

#include <string>

namespace utils {
class FPS {
public:
  static void Tick(const std::string &name);
};
} // namespace utils
#endif