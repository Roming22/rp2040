#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "BaseBoard.h"
#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_col_pins, i_row_pins, true) {}

  static void Setup(const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins);

  void loop();
};
#endif