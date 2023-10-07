#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "../BitBang.hpp"
#include "BaseBoard.hpp"

#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const unsigned int &i_msg_len,
                const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new DaughterBoard(i_msg_len, i_col_pins, i_row_pins);
  }

  void loop() {
    load_switch_events();
    send_switch_events();
    BitBang::ReceiveSync();
  }
};
#endif