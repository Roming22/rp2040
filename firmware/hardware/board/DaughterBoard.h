#ifndef MYMK_HARDWARE_BOARD_DAUGHTERBOARD
#define MYMK_HARDWARE_BOARD_DAUGHTERBOARD

#include "../../logic/ObjectManager.h"
#include "BaseBoard.h"

#include <vector>

namespace hardware {
namespace board {
class DaughterBoard : public hardware::board::BaseBoard {
public:
  typedef std::shared_ptr<DaughterBoard> Ptr;
  static Ptr New(const std::vector<unsigned int> &col_pins,
                 const std::vector<unsigned int> &row_pins);

  DaughterBoard(const std::vector<unsigned int> &i_col_pins,
                const std::vector<unsigned int> &i_row_pins)
      : BaseBoard(i_col_pins, i_row_pins, true) {
    logic::ObjectManager::Register("hardware::board::DaughterBoard");
  }

  static void Setup(const std::vector<unsigned int> &i_col_pins,
                    const std::vector<unsigned int> &i_row_pins);

  void loop();
};
} // namespace board
} // namespace hardware
#endif