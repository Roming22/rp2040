#ifndef MYMK_HARDWARE_BOARD_BASEBOARD
#define MYMK_HARDWARE_BOARD_BASEBOARD

#include "../../logic/ObjectManager.h"
#include "../key/KeyMatrix.h"
#include "../key/KeySwitch.h"

#include <vector>

namespace hardware {
namespace board {
class BaseBoard {
protected:
  typedef std::shared_ptr<BaseBoard> Ptr;
  static BaseBoard::Ptr instance;
  hardware::key::KeySwitch::Ptr key_switch;
  std::vector<int> switch_events;
  bool is_connected;
  bool is_motherboard;

public:
  BaseBoard(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins,
            const bool i_is_connected, const bool i_is_motherboard)
      : switch_events(), is_connected(i_is_connected),
        is_motherboard(i_is_motherboard) {
    key_switch = hardware::key::KeyMatrix::New(i_col_pins, i_row_pins);
    logic::ObjectManager::Register("hardware::board::baseboard");
  }

  static bool IsMotherboard();

  virtual void connect();

  void load_switch_events();

  virtual void tick();

  static void Tick();
};
} // namespace board
} // namespace hardware
#endif