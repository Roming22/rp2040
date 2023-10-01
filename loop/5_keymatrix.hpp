//

#include "../mymk/DaughterBoard.hpp"
#include "../mymk/MotherBoard.hpp"
#include <vector>

void loop_mother() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static MotherBoard board(msgLen, {}, {});
  static std::vector<int> events(2, 0);

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(loopIndex);
  Serial.println(" [Controller]");

  board.load_switch_events(events);
  board.process_switch_events(events);

  ++loopIndex;
}

void loop_daugther() {
  static unsigned int msgLen = 32;
  static DaughterBoard board(msgLen, {}, {});
  static std::vector<int> events(2, 0);

  Serial.println("");
  Serial.print("## Loop ");
  Serial.println(" [Extension]");

  board.load_switch_events(events);
  board.send_switch_events(events);
}

void loop_5_keymatrix() {
  if (isLeft) {
    loop_mother();
  } else {
    loop_daugther();
  }
}
