#include "DaughterBoard.h"

#include "../../utils/Debug.hpp"
#include "../led/Pixels.h"
#include "../txrx/BitBang.hpp"

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
  hardware::led::Pixels::Set(1, 0, 0, 0);
  instance->connect();
}

void DaughterBoard::connect() {
  hardware::led::Pixels::Set(1, 0, 0, 50);
  DEBUG_INFO("DaughterBoard handshake...");
  hardware::txrx::BitBang::Send(key_switch->size);
  DEBUG_INFO("Handshake: OK");
  hardware::led::Pixels::Set(1, 0, 50, 0);
  // TODO: receive layer color
}

void DaughterBoard::send_switch_events() {
  for (int event : switch_events) {
    DEBUG_INFO("Sending switch event %d", event);
    hardware::txrx::BitBang::Send(event);
  }
  hardware::txrx::BitBang::Send(0);
}

void DaughterBoard::tick() {
  load_switch_events();
  send_switch_events();
}
} // namespace board
} // namespace hardware