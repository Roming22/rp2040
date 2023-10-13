#ifndef MYMK_HARDWARE_SWITCH_KEY
#define MYMK_HARDWARE_SWITCH_KEY

#include <vector>

class Key {
public:
  unsigned int size;
  Key(){};
  virtual void poll_events(std::vector<int> &events) {
    DEBUG_VERBOSE("Key.poll_events");
    DEBUG_ERROR("[ERROR] Key.poll_events() is not implemented");
    delay(3600000);
  };
};
#endif