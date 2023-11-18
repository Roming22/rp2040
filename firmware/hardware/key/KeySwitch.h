#ifndef MYMK_HARDWARE_SWITCH_KEY
#define MYMK_HARDWARE_SWITCH_KEY

#include <vector>

namespace hardware {
namespace key {
class KeySwitch {
public:
  unsigned int size;
  KeySwitch(){};
  virtual void poll_events(std::vector<int> &events);
};
} // namespace key
} // namespace hardware
#endif