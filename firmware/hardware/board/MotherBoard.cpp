#include "MotherBoard.h"

#include "../../logic/Events.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Time.h"
#include "../led/Pixels.h"
#include "../txrx/BitBang.hpp"

namespace hardware {
namespace board {
MotherBoard::Ptr MotherBoard::New(const std::vector<unsigned int> &col_pins,
                                  const std::vector<unsigned int> &row_pins,
                                  const bool i_is_left,
                                  const bool is_connected) {
  MotherBoard::Ptr instance =
      Ptr(new MotherBoard(col_pins, row_pins, is_connected));
  instance->is_left = i_is_left;
  return instance;
}

void MotherBoard::Setup(const std::vector<unsigned int> &i_col_pins,
                        const std::vector<unsigned int> &i_row_pins,
                        const bool i_is_left, const bool i_is_connected) {
  instance = New(i_col_pins, i_row_pins, i_is_connected, i_is_left);
  if (i_is_connected) {
    instance->connect();
  }
}

void MotherBoard::connect() {
  hardware::led::Pixels::Set(1, 0, 0, 50);
  DEBUG_INFO("MotherBoard handshake...");
  while (!hardware::txrx::BitBang::Receive()) {
  }
  // TODO: retrieve info from layout (layout.size - key_switch.size)
  int received_offset = hardware::txrx::BitBang::GetValue();
  if (is_left) {
    offset = 0;
    extension_offset = key_switch->size;
  } else {
    offset = received_offset;
    extension_offset = 0;
  }
  // TODO: send layer color
  DEBUG_INFO("Handshake: OK");
  hardware::led::Pixels::Set(1, 0, 50, 0);
  DEBUG_INFO("Connected (offset/extension_offset): %d/%d", offset,
             extension_offset);
}

void MotherBoard::receive_switch_events() {
  DEBUG_VERBOSE("harware::board::MotherBoard::receive_switch_events");
  int event;
  while (hardware::txrx::BitBang::Receive()) {
    event = hardware::txrx::BitBang::GetValue();
    if (event == 0) {
      return;
    } else if (extension_offset > 0) {
      if (event > extension_offset + 1) {
        event -= 1;
        DEBUG_INFO("Received Press event");
      } else {
        event -= 1 + extension_offset * 2;
        DEBUG_INFO("Received Release event");
      }
    }
    switch_events.push_back(event);
  }
}

void MotherBoard::add_events() {
  unsigned long now = utils::Time::Now();
  for (int switch_event : switch_events) {
    std::string event_id;
    if (switch_event > 0) {
      event_id = "switch." + std::to_string(switch_event) + ".pressed";
    } else {
      event_id = "switch." + std::to_string(-switch_event) + ".released";
    }
    logic::Event::Add(event_id, now, nullptr);
  }
}

void MotherBoard::tick() {
  load_switch_events(offset);
  if (extension_offset > 0) {
    receive_switch_events();
  }
  if (!switch_events.empty()) {
    add_events();
  }
}
} // namespace board
} // namespace hardware