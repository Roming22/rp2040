#ifndef MYMK_HARDWARE_KEY_KEYSWITCH
#define MYMK_HARDWARE_KEY_KEYSWITCH

#include <memory>
#include <vector>

namespace hardware {
namespace key {
class KeySwitch {
public:
  typedef std::shared_ptr<KeySwitch> Ptr;
  unsigned int size;
  KeySwitch(){};
  virtual void poll_events(std::vector<int> &events);
};
} // namespace key
} // namespace hardware
#endif