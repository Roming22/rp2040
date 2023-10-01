#ifndef MYMK_BASEBOARD
#define MYMK_BASEBOARD

#include "BitBang.hpp"
#include "KeyMatrix.hpp"
#include <vector>

class BaseBoard {
protected:
  KeyMatrix keymatrix;
  unsigned int msg_len;
  bool is_connected;

public:
  BaseBoard(const unsigned int &i_msg_len, const unsigned int i_col_pins[],
            const unsigned int i_row_pins[])
      : msg_len(i_msg_len), is_connected(true) {
    std::vector<unsigned int> col_pins({26, 22, 20, 23});
    std::vector<unsigned int> row_pins({6, 7, 9});
    keymatrix = KeyMatrix(col_pins, row_pins);
  }

  void send_switch_events(const std::vector<int> &events) {
    if (!is_connected) {
      return;
    }

    for (unsigned int i = 0; i < events.size(); ++i) {
      // Serial.println("POST Value");
      BitBang::send(events[i], msg_len);
    }
    // Serial.println("POST Value");
    BitBang::send(0, msg_len);
  }

  virtual void receive_switch_events(std::vector<int> &events) {
    int event = 1;
    while (event != 0 && is_connected) {
      // Serial.println("GET Value");
      event = BitBang::receive(msg_len);
      if (event > 0) {
        events.push_back(event + keymatrix.size);
      } else if (event < 0) {
        events.push_back(event - keymatrix.size);
      }
    }
  }

  void process_switch_events(const std::vector<int> &events) {
    // Serial.println("Process switch events");
    int event;
    // Process events
    for (int i = 0; i < events.size(); ++i) {
      event = events[i];
      Serial.print("Switch Event: ");
      Serial.println(event);
      blinkLeds(100);
    }
  }
};

#endif