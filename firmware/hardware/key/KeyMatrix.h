#ifndef MYMK_HARDWARE_KEY_KEYMATRIX
#define MYMK_HARDWARE_KEY_KEYMATRIX

#include "KeySwitch.h"

#include <memory>
#include <vector>

namespace hardware {
namespace key {
class KeyMatrix : public KeySwitch {
public:
  typedef std::shared_ptr<KeyMatrix> Ptr;
  static Ptr New(const std::vector<unsigned int> &col_pins,
                 const std::vector<unsigned int> &row_pins);

private:
  std::vector<unsigned int> col_pins;
  std::vector<unsigned int> row_pins;
  std::vector<bool> key_states;
  unsigned long last_poll;
  unsigned int debounce_delay;

  void set_timer();

public:
  KeyMatrix(const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins);

  void poll_events(std::vector<int> &events);
};
} // namespace key
} // namespace hardware
#endif