#ifndef MYMK_UTILS_FPS
#define MYMK_UTILS_FPS

#include <functional>
#include <string>

namespace utils {
class FPS {
public:
  static int delay;
  static void Tick(const std::string &name);
  static void TimeIt(std::function<void()>);
};
} // namespace utils
#endif