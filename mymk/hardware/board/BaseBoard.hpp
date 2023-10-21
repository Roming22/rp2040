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
  bool is_connected;
  bool is_motherboard;

public:
  BaseBoard(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins,
            const bool i_is_connected)
      : is_connected(i_is_connected) {
    key = new KeyMatrix(i_col_pins, i_row_pins);
  }

  void load_switch_events(std::vector<int> &switch_events) {
    DEBUG_VERBOSE("BaseBoard::load_switch_events");
    key->poll_events(switch_events);
  }

  virtual void tick() {
    DEBUG_ERROR("[ERROR] BaseBoard::tick is not implemented");
    delay(3600000);
  }

  static void Tick() {
    DEBUG_VERBOSE("BaseBoard::Tick");
    if (instance->is_connected) {
      BitBang::Tick();
    }
    instance->tick();
  };
};
BaseBoard *BaseBoard::instance = nullptr;
#endif