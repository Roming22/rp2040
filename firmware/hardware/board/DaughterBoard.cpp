#include "DaughterBoard.h"

#include "../../utils/Debug.hpp"

namespace hardware {
namespace board {
DaughterBoard::Ptr
DaughterBoard::New(const std::vector<unsigned int> &col_pins,
                   const std::vector<unsigned int> &row_pins) {
  return Ptr(new DaughterBoard(col_pins, row_pins));
}

void DaughterBoard::Setup(const std::vector<unsigned int> &i_col_pins,
                          const std::vector<unsigned int> &i_row_pins) {
  instance = DaughterBoard::New(i_col_pins, i_row_pins);
  hardware::txrx::BitBang::Send(i_col_pins.size() * i_row_pins.size());
}

void DaughterBoard::loop() { load_switch_events(); }
} // namespace board
} // namespace hardware