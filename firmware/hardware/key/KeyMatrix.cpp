#include "KeyMatrix.h"

#include "../../logic/ObjectManager.h"
#include "../../utils/Debug.hpp"
#include "../../utils/Time.h"

namespace hardware {
namespace key {
KeyMatrix::Ptr KeyMatrix::New(const std::vector<unsigned int> &col_pins,
                              const std::vector<unsigned int> &row_pins) {
  return Ptr(new KeyMatrix(col_pins, row_pins));
}

KeyMatrix::KeyMatrix(const std::vector<unsigned int> &i_col_pins,
                     const std::vector<unsigned int> &i_row_pins)
    : last_poll(0), debounce_delay(15000) {
  col_pins = std::vector(i_col_pins);
  row_pins = std::vector(i_row_pins);
  size = col_pins.size() * row_pins.size();
  DEBUG_DEBUG("KeyMatrix size: %d", size);
  key_states = std::vector<bool>(size, HIGH);

  // Set col pins
  for (unsigned int col_pin : col_pins) {
    pinMode(col_pin, INPUT_PULLUP);
  }

  // Set row pins
  for (unsigned int row_pin : row_pins) {
    pinMode(row_pin, INPUT);
  }

  logic::ObjectManager::Register("hardware::key::KeyMatrix");
}

void KeyMatrix::poll_events(std::vector<int> &events) {
  // DEBUG_INFO("hardware::key::KeyMatrix::poll_events");
  unsigned long now = utils::Time::Now();
  if (now - last_poll < debounce_delay) {
    // DEBUG_INFO("Debouncing %d - %d < %d", now, last_poll, debounce_delay);
    return;
  }

  // DEBUG_INFO("hardware::key::KeyMatrix::poll_events Scanning");
  bool state;
  int key_index = 0;
  for (unsigned int row_pin : row_pins) {
    pinMode(row_pin, OUTPUT);
    digitalWrite(row_pin, LOW);
    for (unsigned int col_pin : col_pins) {
      state = digitalRead(col_pin);
      if (state != key_states[key_index]) {
        if (state == LOW) {
          // DEBUG_INFO("hardware::key::KeyMatrix::poll_events Switch %d
          // pressed",
          //            key_index + 1);
          events.push_back(1 + key_index);
        } else {
          // DEBUG_INFO("hardware::key::KeyMatrix::poll_events Switch %d
          // released",
          //            key_index + 1);
          events.push_back(-1 - key_index);
        }
        key_states[key_index] = state;
        last_poll = now;
      }
      ++key_index;
    }
    pinMode(row_pin, INPUT);
  }
}
} // namespace key
} // namespace hardware