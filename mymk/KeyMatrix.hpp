#ifndef MYMK_KEYMATRIX
#define MYMK_KEYMATRIX

#include <vector>

class KeyMatrix {
private:
  std::vector<unsigned int> col_pins;
  std::vector<unsigned int> row_pins;
  std::vector<bool> key_states;

public:
  KeyMatrix(){};
  KeyMatrix(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins) {
    col_pins = std::vector(i_col_pins);
    row_pins = std::vector(i_row_pins);
    key_states = std::vector<bool>(col_pins.size() * row_pins.size(), HIGH);

    // Set col pins
    for (unsigned int col = 0; col < col_pins.size(); ++col) {
      pinMode(col_pins[col], INPUT_PULLUP);
    }
    // Set row pins
    for (unsigned int row; row < row_pins.size(); ++row) {
      pinMode(row_pins[row], INPUT);
    }
  }

  void poll_switch_events(std::vector<int> &events) {
    Serial.println("Polling switch events");
    bool state;
    int key_index = 0;

    for (unsigned int row = 0; row < row_pins.size(); ++row) {
      pinMode(row_pins[row], OUTPUT);
      digitalWrite(row_pins[row], LOW);
      for (unsigned int col = 0; col < col_pins.size(); col++) {
        state = digitalRead(col_pins[col]);
        if (state != key_states[key_index]) {
          if (state == LOW) {
            // Serial.println("Pressed");
            events.push_back(1 + key_index);
          } else {
            // Serial.println("Released");
            events.push_back(-1 - key_index);
          }
          key_states[key_index] = state;
        }
        ++key_index;
      }
      pinMode(row_pins[row], INPUT);
    }
  }
};

#endif