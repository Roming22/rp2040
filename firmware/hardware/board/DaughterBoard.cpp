#include "DaughterBoard.h"
#include "../../utils/Debug.hpp"

namespace hardware {
namespace board {
void DaughterBoard::Setup(const std::vector<unsigned int> &i_col_pins,
                          const std::vector<unsigned int> &i_row_pins) {
  instance = new DaughterBoard(i_col_pins, i_row_pins);
  hardware::txrx::BitBang::Send(i_col_pins.size() * i_row_pins.size());
}

void DaughterBoard::loop() {
  std::vector<int> switch_events(2, 0);
  load_switch_events(switch_events);
}
} // namespace board
} // namespace hardware