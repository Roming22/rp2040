#ifndef MYMK_HARDWARE_BOARD_BASEBOARD
#define MYMK_HARDWARE_BOARD_BASEBOARD

#include "../../logic/Timer.hpp"
#include "../BitBang.hpp"
#include "../switch/Key.hpp"
#include "../switch/KeyMatrix.hpp"

#include <vector>

class BaseBoard {
protected:
  static BaseBoard *instance;
  Key *key;
  unsigned int msg_len;
  bool is_connected;
  bool is_motherboard;

public:
  BaseBoard(const unsigned int &i_msg_len,
            const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins)
      : msg_len(i_msg_len), is_connected(true) {
    key = new KeyMatrix(i_col_pins, i_row_pins);
  }

  void load_switch_events(std::vector<int> &switch_events) {
    // Serial.println("BaseBoard::load_switch_events");
    switch_events.clear();
    key->poll_events(switch_events);
  }

  virtual void loop() {
    Serial.println("[ERROR] BaseBoard::loop is not implemented");
    delay(3600000);
  }

  static void Tick() {
    // Serial.println("BaseBoard::Tick");
    instance->loop();
  };
};
BaseBoard *BaseBoard::instance = nullptr;
#endif