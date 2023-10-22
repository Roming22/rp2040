#include "DaughterBoard.h"
#include "../../utils/Debug.hpp"

void DaughterBoard::Setup(const std::vector<unsigned int> &i_col_pins,
                          const std::vector<unsigned int> &i_row_pins) {
  instance = new DaughterBoard(i_col_pins, i_row_pins);
  BitBang::Send(i_col_pins.size() * i_row_pins.size());
}

void DaughterBoard::loop() {
  std::vector<int> switch_events(2, 0);
  load_switch_events(switch_events);
}
