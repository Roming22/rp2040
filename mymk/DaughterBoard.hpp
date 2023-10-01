#ifndef MYMK_DAUGHTERBOARD
#define MYMK_DAUGHTERBOARD

#include "BaseBoard.hpp"
#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const unsigned int &i_msg_len, const unsigned int i_col_pins[],
                const unsigned int i_row_pins[])
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}
  void load_switch_events(std::vector<int> &events) {
    events.clear();

    // Get events on the motherboard
    keymatrix.poll_switch_events(events);
  }

  void receive_switch_events(std::vector<int> &events) override {
    events.clear();
    BaseBoard::receive_switch_events(events);
  }
};

#endif