#include "MotherBoard.h"

#include "../../logic/Events.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Time.h"

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
}

void MotherBoard::receive_switch_events(std::vector<int> &switch_events) {
  DEBUG_VERBOSE("harware::board::MotherBoard::receive_switch_events");
  std::queue<unsigned int> &daughterboard_switch_events =
      hardware::txrx::BitBang::In();
  while (daughterboard_switch_events.size() > 0) {
    int event = daughterboard_switch_events.front();
    daughterboard_switch_events.pop();
    if (event > 0) {
      DEBUG_INFO("Received Press event");
      switch_events.push_back(event + key_switch->size);
    } else {
      DEBUG_INFO("Received Release event");
      switch_events.push_back(event - key_switch->size);
    }
  }
}

void MotherBoard::add_events(const std::vector<int> &switch_events) {
  unsigned long now = micros();
  for (unsigned int i = 0; i < switch_events.size(); ++i) {
    int switch_event = switch_events[i];
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
  std::vector<int> switch_events(2, 0);
  load_switch_events(switch_events);
  if (is_connected) {
    receive_switch_events(switch_events);
  }
  add_events(switch_events);
}
} // namespace board
} // namespace hardware