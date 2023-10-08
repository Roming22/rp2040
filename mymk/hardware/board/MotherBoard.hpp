#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "../BitBang.hpp"
#include "BaseBoard.hpp"

#include <vector>

class MotherBoard : public BaseBoard {
public:
  MotherBoard(const unsigned int &i_msg_len,
              const std::vector<unsigned int> i_col_pins,
              const std::vector<unsigned int> i_row_pins)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins) {}

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins) {
    instance = new MotherBoard(i_msg_len, i_col_pins, i_row_pins);
  }

  void receive_switch_events() {
    // Serial.println("BaseBoard::receive_switch_events");
    int event = 1;
    while (event != 0 && is_connected) {
      // Serial.println("GET Value");
      event = BitBang::Receive(msg_len);
      if (event > 0) {
        Serial.println("Received Press event");
        switch_events.push_back(event + key->size);
      } else if (event < 0) {
        Serial.println("Received Release event");
        switch_events.push_back(event - key->size);
      }
    }
  }

  void process_switch_events() {
    // Serial.println("BaseBoard::process_switch_events");
    int event;
    for (int i = 0; i < switch_events.size(); ++i) {
      event = switch_events[i];
      Serial.print("Switch Event: ");
      Serial.println(event);
      Pixels::FlipFlop(messages);
    }
  }

  void send_messages() {
    // Serial.println("BaseBoard::send_messages");
    if (!is_connected) {
      return;
    }

    unsigned int message;
    for (unsigned int i = 0; i < messages.size(); ++i) {
      // Serial.println("POST Value");
      message = messages[i];
      BitBang::Send(message, msg_len);
    }
    // Serial.println("POST Value");
    BitBang::Send(0, msg_len);
    messages.clear();
  }

  void loop() {
    load_switch_events();
    receive_switch_events();
    process_switch_events();
    send_messages();
  }
};
#endif