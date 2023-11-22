#ifndef MYMK_HARDWARE_KEY_KEYSWITCH
#define MYMK_HARDWARE_KEY_KEYSWITCH

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