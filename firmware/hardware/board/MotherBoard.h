#ifndef MYMK_HARDWARE_BOARD_MOTHERBOARD
#define MYMK_HARDWARE_BOARD_MOTHERBOARD

#include "../../logic/ObjectManager.h"
#include "BaseBoard.h"

#include <vector>

namespace hardware {
namespace board {
class MotherBoard : public hardware::board::BaseBoard {
public:
  typedef std::shared_ptr<MotherBoard> Ptr;

  static Ptr New(const std::vector<unsigned int> &col_pins,
                 const std::vector<unsigned int> &row_pins,
                 const bool is_connected);

  MotherBoard(const std::vector<unsigned int> &i_col_pins,
              const std::vector<unsigned int> &i_row_pins,
              const bool i_is_connected)
      : BaseBoard(i_col_pins, i_row_pins, i_is_connected) {
    logic::ObjectManager::Register("hardware::board::MotherBoard");
  }

  static void Setup(const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins,
                    const bool i_is_connected);

  void receive_switch_events();

  void add_events();

  void tick();
};
} // namespace board
} // namespace hardware
#endif