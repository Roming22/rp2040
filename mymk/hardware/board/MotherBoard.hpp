#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "../led/Pixels.hpp"
#include "BaseBoard.hpp"

#include <vector>

class MotherBoard : public BaseBoard {
public:
  MotherBoard(const unsigned int &i_msg_len,
              const std::vector<unsigned int> &i_col_pins,
              const std::vector<unsigned int> &i_row_pins,
              const bool i_is_connected)
      : BaseBoard(i_msg_len, i_col_pins, i_row_pins, i_is_connected) {}

  static void Setup(const unsigned int &i_msg_len,
                    const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins,
                    const bool i_is_connected) {
    instance =
        new MotherBoard(i_msg_len, i_col_pins, i_row_pins, i_is_connected);
  }

  void process_timer_events(std::vector<std::string> timer_events) {
    DEBUG_VERBOSE("MotherBoard::process_timer_events");
    std::string event;
    for (int i = 0; i < timer_events.size(); ++i) {
      event = timer_events[i];
      DEBUG_INFO("Timer Event: %d", event.c_str());
    }
    timer_events.clear();
  }

  void receive_switch_events(std::vector<int> &switch_events) {
    DEBUG_VERBOSE("MotherBoard::receive_switch_events");
    int event = 1;
    while (event != 0 && is_connected) {
      DEBUG_DEBUG("GET Value");
      event = BitBang::Receive(msg_len);
      if (event > 0) {
        DEBUG_INFO("Received Press event");
        switch_events.push_back(event + key->size);
      } else if (event < 0) {
        DEBUG_INFO("Received Release event");
        switch_events.push_back(event - key->size);
      }
    }
  }

  void process_switch_events(const std::vector<int> &switch_events,
                             std::vector<unsigned int> &messages,
                             std::vector<std::string> timer_events) {
    DEBUG_VERBOSE("MotherBoard::process_switch_events");
    messages.clear();
    int event;
    for (int i = 0; i < switch_events.size(); ++i) {
      event = switch_events[i];
      DEBUG_INFO("Switch Event: %d", event);
      messages.push_back(Pixels::FlipFlop());
    }
  }

  void send_messages(const std::vector<unsigned int> &messages) {
    DEBUG_VERBOSE("MotherBoard::send_switch_events");
    if (!is_connected) {
      return;
    }

    unsigned int message;
    for (unsigned int i = 0; i < messages.size(); ++i) {
      DEBUG_DEBUG("POST Value");
      message = messages[i];
      DEBUG_INFO("Send message: %d", message);
      BitBang::Send(message, msg_len);
    }
    DEBUG_DEBUG("POST Value");
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