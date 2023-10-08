#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "../BitBang.hpp"
#include "BaseBoard.hpp"

#include <vector>

class DaughterBoard : public BaseBoard {
public:
  DaughterBoard(const unsigned int &i_msg_len,
                const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new DaughterBoard(i_msg_len, i_col_pins, i_row_pins);
  }

  void send_switch_events() {
    // Serial.println("BaseBoard::send_switch_events");
    if (!is_connected) {
      return;
    }

    int event;
    for (unsigned int i = 0; i < switch_events.size(); ++i) {
      // Serial.println("POST Value");
      event = switch_events[i];
      Serial.print("Send event: ");
      Serial.println(event);
      BitBang::Send(event, msg_len);
    }
    // Serial.println("POST Value");
    BitBang::Send(0, msg_len);
  }

  void receive_messages() {
    // Serial.println("BaseBoard::receive_messages");
    unsigned int data = 1;
    while (data != 0 && is_connected) {
      // Serial.println("GET Value");
      data = BitBang::Receive(msg_len);
      if (data > 0) {
        messages.push_back(data);
      }
    }
  }

  void process_messages() {
    // Serial.println("BaseBoard::process_messages");
    unsigned int data;
    for (int i = 0; i < messages.size(); ++i) {
      data = messages[i];
      unsigned int blue = data & 255;
      unsigned int green = (data >> 8) & 255;
      unsigned int red = (data >> 16) & 255;
      unsigned int address = data >> 24;
      Pixels::Set(address, red, green, blue);
    }
    messages.clear();
  }

  void loop() {
    load_switch_events();
    send_switch_events();
    receive_messages();
    process_messages();
  }
};
#endif