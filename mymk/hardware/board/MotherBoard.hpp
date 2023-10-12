#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "BaseBoard.hpp"
#include <vector>

class MotherBoard : public BaseBoard {
public:
  MotherBoard(const unsigned int &i_msg_len,
              const std::vector<unsigned int> i_col_pins,
              const std::vector<unsigned int> i_row_pins)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new MotherBoard(i_msg_len, i_col_pins, i_row_pins);
  }

  void loop() {
    static std::vector<int> events(2, 0);
    load_switch_events(events);
    receive_switch_events(events);
    process_switch_events(events);
    send_switch_events(events);
  }
};
#endif