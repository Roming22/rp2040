#ifndef MYMK_DAUGHTERBOARD
#define MYMK_DAUGHTERBOARD

#include "BitBang.hpp"
#include "KeyMatrix.hpp"
#include <vector>

class DaughterBoard {
private:
  KeyMatrix keymatrix;
  unsigned int msg_len;
  bool is_motherboard_online;

public:
  DaughterBoard(const unsigned int &i_msg_len, const unsigned int i_col_pins[],
                const unsigned int i_row_pins[])
      : msg_len(i_msg_len), is_motherboard_online(false) {
    std::vector<unsigned int> col_pins({23, 20, 22, 26});
    std::vector<unsigned int> row_pins({6, 7, 9});
    keymatrix = KeyMatrix(col_pins, row_pins);
  }

  void load_switch_events(std::vector<int> &events) {
    events.clear();

    // Get events on the motherboard
    keymatrix.poll_switch_events(events);
  }

  void send_switch_events(const std::vector<int> &events) {
    if (!is_motherboard_online) {
      return;
    }

    for (unsigned int i = 0; i < events.size(); ++i) {
      Serial.println("POST Value");
      BitBang::send(events[i], msg_len);
    }
    Serial.println("POST Value");
    BitBang::send(0, msg_len);
  }
};

#endif