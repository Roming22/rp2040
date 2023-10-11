#ifndef MYMK_HARDWARE_KEYMATRIX
#define MYMK_HARDWARE_KEYMATRIX

#include "Key.hpp"
#include <queue>
#include <vector>

class KeyMatrix : public Key {
private:
  std::vector<unsigned int> col_pins;
  std::vector<unsigned int> row_pins;
  std::vector<bool> key_states;

public:
  KeyMatrix(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins) {
    col_pins = std::vector(i_col_pins);
    row_pins = std::vector(i_row_pins);
    size = col_pins.size() * row_pins.size();
    Serial.println("KeyMatrix size:");
    Serial.println(size);
    delay(2000);
    key_states = std::vector<bool>(size, HIGH);

    // Set col pins
    // Serial.print("Col pin: ");
    for (unsigned int col = 0; col < col_pins.size(); ++col) {
      // if (col != 0) {
      //   Serial.print(", ");
      // }
      // Serial.print(col_pins[col]);
      pinMode(col_pins[col], INPUT_PULLUP);
    }
    // Serial.println("");

    // Set row pins
    // Serial.print("Row pin: ");
    for (unsigned int row = 0; row < row_pins.size(); ++row) {
      // if (row != 0) {
      //   Serial.print(", ");
      // }
      // Serial.print(row_pins[row]);
      pinMode(row_pins[row], INPUT);
    }
    // Serial.println("");
  }

  void poll_events(std::vector<int> &events) {
    // Serial.println("KeyMatrix.poll_events");
    bool state;
    int key_index = 0;

    if (!events.empty()) {
      Serial.println("[ERROR] Switch events is not empty");
      delay(3000);
    }

    for (unsigned int row = 0; row < row_pins.size(); ++row) {
      pinMode(row_pins[row], OUTPUT);
      digitalWrite(row_pins[row], LOW);
      for (unsigned int col = 0; col < col_pins.size(); col++) {
        state = digitalRead(col_pins[col]);
        if (state != key_states[key_index]) {
          Serial.print("COL pin: ");
          Serial.print(col_pins[col]);
          Serial.print("    ROW pin: ");
          Serial.print(row_pins[row]);
          Serial.print("    Before: ");
          Serial.print(key_states[key_index]);
          if (state == LOW) {
            Serial.print("    Switch pressed: ");
            Serial.print(key_index + 1);
            events.push_back(1 + key_index);
          } else {
            Serial.print("    Switch released: ");
            Serial.print(key_index + 1);
            events.push_back(-1 - key_index);
          }
          key_states[key_index] = state;
          Serial.print("    After: ");
          Serial.println(key_states[key_index]);
        }
        ++key_index;
      }
      pinMode(row_pins[row], INPUT);
    }
  }
};

#endif