#ifndef MYMK_MOTHERBOARD
#define MYMK_MOTHERBOARD

#include "BitBang.hpp"
#include "KeyMatrix.hpp"
#include <vector>

class MotherBoard {
private:
  KeyMatrix keymatrix;
  unsigned int msg_len;
  bool is_daughterboard_online;

public:
  MotherBoard(const unsigned int &i_msg_len, const unsigned int i_col_pins[],
              const unsigned int i_row_pins[])
      : msg_len(i_msg_len), is_daughterboard_online(false) {
    std::vector<unsigned int> col_pins({26, 22, 20, 23});
    std::vector<unsigned int> row_pins({6, 7, 9});
    keymatrix = KeyMatrix(col_pins, row_pins);
  }

  void load_switch_events(std::vector<int> &events) {
    events.clear();

    // Get events on the motherboard
    keymatrix.poll_switch_events(events);

    // Get events on the daughterboard
    receive_switch_events(events);
  }

  void receive_switch_events(std::vector<int> &events) {
    int event = 1;
    while (event != 0 && is_daughterboard_online) {
      Serial.println("GET Value");
      event = BitBang::receive(msg_len);
      if (event != 0) {
        events.push_back(event);
      }
    }
  }

  void process_switch_events(const std::vector<int> &events) {
    Serial.println("Process switch events");
    int event;
    // Process events
    for (int i = 0; i < events.size(); ++i) {
      event = events[i];
      Serial.print("Key Event: ");
      Serial.println(event);
      blinkLeds(100);
    }
  }
};

#endif