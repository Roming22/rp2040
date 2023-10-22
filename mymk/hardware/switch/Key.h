#ifndef MYMK_HARDWARE_SWITCH_KEY
#define MYMK_HARDWARE_SWITCH_KEY

#include <vector>

class Key {
public:
  unsigned int size;
  Key(){};
  virtual void poll_events(std::vector<int> &events);
};
#endif