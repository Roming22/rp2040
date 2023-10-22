#ifndef MYMK_HARDWARE_KEYMATRIX
#define MYMK_HARDWARE_KEYMATRIX

#include "Key.h"

#include <vector>

class KeyMatrix : public Key {
private:
  std::vector<unsigned int> col_pins;
  std::vector<unsigned int> row_pins;
  std::vector<bool> key_states;

  void set_timer();

public:
  bool can_poll;
  KeyMatrix(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins);

  void poll_events(std::vector<int> &events);
};
#endif