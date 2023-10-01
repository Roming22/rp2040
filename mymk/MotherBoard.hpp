#ifndef MYMK_MOTHERBOARD
#define MYMK_MOTHERBOARD

#include "BaseBoard.hpp"
#include <vector>

class MotherBoard : public BaseBoard {
public:
  MotherBoard(const unsigned int &i_msg_len, const unsigned int i_col_pins[],
              const unsigned int i_row_pins[])
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}
  void load_switch_events(std::vector<int> &events) {
    events.clear();

    // Get events on the motherboard
    keymatrix.poll_switch_events(events);

    // Get events on the daughterboard
    receive_switch_events(events);
  }
};

#endif