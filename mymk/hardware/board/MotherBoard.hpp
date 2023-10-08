#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "../led/Pixels.hpp"
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

  void process_timer_events(std::vector<std::string> timer_events) {
    // Serial.println("MotherBoard::process_timer_events");
    std::string event;
    for (int i = 0; i < timer_events.size(); ++i) {
      event = timer_events[i];
      Serial.print("Timer Event: ");
      Serial.println(event.c_str());
    }
    timer_events.clear();
  }

  void receive_switch_events(std::vector<int> &switch_events) {
    // Serial.println("MotherBoard::receive_switch_events");
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

  void process_switch_events(const std::vector<int> &switch_events,
                             std::vector<unsigned int> &messages,
                             std::vector<std::string> timer_events) {
    // Serial.println("MotherBoard::process_switch_events");
    messages.clear();
    int event;
    for (int i = 0; i < switch_events.size(); ++i) {
      event = switch_events[i];
      Serial.print("Switch Event: ");
      Serial.println(event);
      messages.push_back(Pixels::FlipFlop());
    }
  }

  void send_messages(const std::vector<unsigned int> &messages) {
    // Serial.println("MotherBoard::send_switch_events");
    if (!is_connected) {
      return;
    }

    unsigned int message;
    for (unsigned int i = 0; i < messages.size(); ++i) {
      // Serial.println("POST Value");
      message = messages[i];
      Serial.print("Send message: ");
      Serial.println(message);
      BitBang::Send(message, msg_len);
    }
    // Serial.println("POST Value");
    BitBang::Send(0, msg_len);
  }

  void loop() {
    static std::vector<int> switch_events(2, 0);
    static std::vector<std::string> timer_events;
    static std::vector<unsigned int> messages(2, 0);
    process_timer_events(timer_events);
    load_switch_events(switch_events);
    receive_switch_events(switch_events);
    process_switch_events(switch_events, messages, timer_events);
    send_messages(messages);
  }
};
#endif