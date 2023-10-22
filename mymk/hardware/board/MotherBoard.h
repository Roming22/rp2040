#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "BaseBoard.h"

#include <vector>

class MotherBoard : public BaseBoard {
public:
  MotherBoard(const std::vector<unsigned int> &i_col_pins,
              const std::vector<unsigned int> &i_row_pins,
              const bool i_is_connected)
      : BaseBoard(i_col_pins, i_row_pins, i_is_connected) {}

  static void Setup(const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins,
                    const bool i_is_connected);

  void receive_switch_events(std::vector<int> &switch_events);

  void add_events(const std::vector<int> &switch_events);

  void tick();
};
#endif