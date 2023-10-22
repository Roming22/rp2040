#include "KeyMatrix.h"
#include "../../utils/Debug.hpp"

KeyMatrix::KeyMatrix(const std::vector<unsigned int> &i_col_pins,
                     const std::vector<unsigned int> &i_row_pins) {
  col_pins = std::vector(i_col_pins);
  row_pins = std::vector(i_row_pins);
  size = col_pins.size() * row_pins.size();
  DEBUG_DEBUG("KeyMatrix size: %d", size);
  key_states = std::vector<bool>(size, HIGH);

  // Set col pins
  for (unsigned int col = 0; col < col_pins.size(); ++col) {
    pinMode(col_pins[col], INPUT_PULLUP);
  }

  // Set row pins
  for (unsigned int row = 0; row < row_pins.size(); ++row) {
    pinMode(row_pins[row], INPUT);
  }
}

void KeyMatrix::poll_events(std::vector<int> &events) {
  DEBUG_VERBOSE("KeyMatrix.poll_events");
  bool state;
  int key_index = 0;

  events.clear();
  for (unsigned int row = 0; row < row_pins.size(); ++row) {
    pinMode(row_pins[row], OUTPUT);
    digitalWrite(row_pins[row], LOW);
    for (unsigned int col = 0; col < col_pins.size(); col++) {
      state = digitalRead(col_pins[col]);
      if (state != key_states[key_index]) {
        DEBUG_DEBUG("COL pin: %d    ROW pin: %d    Before: %d", col_pins[col],
                    row_pins[row], key_states[key_index]);
        if (state == LOW) {
          DEBUG_DEBUG("Switch pressed: %d", key_index + 1);
          events.push_back(1 + key_index);
        } else {
          DEBUG_DEBUG("Switch released: %d", key_index + 1);
          events.push_back(-1 - key_index);
        }
        key_states[key_index] = state;
        DEBUG_DEBUG("COL pin: %d    ROW pin: %d    After: %d", col_pins[col],
                    row_pins[row], key_states[key_index]);
      }
      ++key_index;
    }
    pinMode(row_pins[row], INPUT);
  }
}
