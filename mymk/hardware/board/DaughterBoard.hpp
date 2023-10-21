#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "../led/Pixels.hpp"
#include "BaseBoard.hpp"
#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_col_pins, i_row_pins, true) {}

  static void Setup(const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new DaughterBoard(i_col_pins, i_row_pins);
    BitBang::Send(i_col_pins.size() * i_row_pins.size());
  }

  void loop() {
    std::vector<int> switch_events(2, 0);
    load_switch_events(switch_events);
  }
};
#endif