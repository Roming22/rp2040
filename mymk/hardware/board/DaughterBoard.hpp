#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "BaseBoard.hpp"
#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const unsigned int &i_msg_len,
                const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}
  void load_switch_events(std::vector<int> &events) {
    events.clear();

    // Get events on the motherboard
    key->poll_events(events);
  }

  void receive_switch_events(std::vector<int> &events) override {
    events.clear();
    BaseBoard::receive_switch_events(events);
  }

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new DaughterBoard(i_msg_len, i_col_pins, i_row_pins);
  }

  void loop() {
    static std::vector<int> events(2, 0);
    load_switch_events(events);
    send_switch_events(events);
    receive_switch_events(events);
    process_switch_events(events);
  }
};
#endif