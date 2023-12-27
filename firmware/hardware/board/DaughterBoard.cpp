#include "DaughterBoard.h"

#include "../../utils/Debug.hpp"
#include "../led/Pixels.h"
#include "../txrx/BitBang.h"

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
  instance->connect();
}

void DaughterBoard::connect() {
  delay(100);
  hardware::led::Pixels::Set(1, 0, 0, 50);
  hardware::txrx::BitBang::SendData(key_switch->size);
  hardware::led::Pixels::Set(1, 0, 0, 0);
  // TODO: receive layer color
}

void DaughterBoard::loop() { load_switch_events(); }
} // namespace board
} // namespace hardware