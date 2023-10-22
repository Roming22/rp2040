#ifndef MYMK_HARDWARE_BOARD_BASEBOARD
#define MYMK_HARDWARE_BOARD_BASEBOARD

#include "../../logic/Timer.h"
#include "../BitBang.h"
#include "../switch/Key.h"
#include "../switch/KeyMatrix.h"

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

  void load_switch_events(std::vector<int> &switch_events);

  virtual void tick();

  static void Tick();
};
#endif