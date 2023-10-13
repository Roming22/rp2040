#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "../led/Pixels.hpp"
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

  void send_switch_events(const std::vector<int> &switch_events) {
    DEBUG_VERBOSE("DaughterBoard::send_switch_events");
    if (!is_connected) {
      return;
    }

    int event;
    for (unsigned int i = 0; i < switch_events.size(); ++i) {
      DEBUG_DEBUG("POST Value");
      event = switch_events[i];
      DEBUG_INFO("Send event: %d", event);
      BitBang::Send(event, msg_len);
    }
    DEBUG_DEBUG("POST Value");
    BitBang::Send(0, msg_len);
  }

  void receive_messages(std::vector<unsigned int> &messages) {
    DEBUG_VERBOSE("DaughterBoard::receive_messages");
    messages.clear();
    unsigned int message = 1;
    while (message != 0 && is_connected) {
      DEBUG_DEBUG("GET Value");
      message = BitBang::Receive(msg_len);
      if (message != 0) {
        DEBUG_INFO("Received message: %d", message);
        messages.push_back(message);
      }
    }
  }

  void process_messages(const std::vector<unsigned int> &messages) {
    DEBUG_VERBOSE("DaughterBoard::process_messages");
    int data;
    for (int i = 0; i < messages.size(); ++i) {
      data = messages[i];
      unsigned int blue = data & 255;
      unsigned int green = (data >> 8) & 255;
      unsigned int red = (data >> 16) & 255;
      unsigned int address = data >> 24;
      Pixels::Set(address, red, green, blue);
      Timer::Start("revertPixel#1ToWhite", 1000,
                   []() { Pixels::Set(1, 255, 255, 255); });
    }
  }

  void loop() {
    static std::vector<int> switch_events(2, 0);
    static std::vector<unsigned int> messages(2, 0);
    messages.clear();
    load_switch_events(switch_events);
    send_switch_events(switch_events);
    receive_messages(messages);
    process_messages(messages);
  }
};
#endif