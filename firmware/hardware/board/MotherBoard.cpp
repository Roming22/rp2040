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
                                  const bool is_connected) {
  return Ptr(new MotherBoard(col_pins, row_pins, is_connected));
}

void MotherBoard::Setup(const std::vector<unsigned int> &i_col_pins,
                        const std::vector<unsigned int> &i_row_pins,
                        const bool i_is_connected) {
  instance = New(i_col_pins, i_row_pins, i_is_connected);
  if (i_is_connected) {
    instance->connect();
  }
}

void MotherBoard::connect() {
  hardware::led::Pixels::Set(1, 0, 0, 50);
  DEBUG_INFO("MotherBoard handshake...");
  while (!hardware::txrx::BitBang::Receive()) {
  }
  offset = hardware::txrx::BitBang::GetValue();
  // TODO: send layer color
  DEBUG_INFO("Handshake: OK");
  hardware::led::Pixels::Set(1, 0, 50, 0);
  DEBUG_INFO("Connected (offset): %d", offset);
}

void MotherBoard::receive_switch_events() {
  DEBUG_VERBOSE("harware::board::MotherBoard::receive_switch_events");
  int event;
  while (true) {
    event = hardware::txrx::BitBang::Receive();
    if (event == 0) {
      return;
    }
    if (event > 0) {
      DEBUG_INFO("Received Press event");
      switch_events.push_back(event + offset);
    } else {
      DEBUG_INFO("Received Release event");
      switch_events.push_back(event - offset);
    }
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
  switch_events.clear();
  load_switch_events();
  if (offset > 0) {
    receive_switch_events();
  }
  if (!switch_events.empty()) {
    add_events();
  }
}
} // namespace board
} // namespace hardware