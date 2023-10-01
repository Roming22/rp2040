#ifndef MYMK_BOARD
#define MYMK_BOARD

#include "config/Loader.hpp"

void set_board() {
  int len = 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
  char board_uid[len] = "";
  pico_get_unique_board_id_string(board_uid, len);
  std::string left_uid("E461508723193526");
  isLeft = std::string(board_uid) == left_uid;

  Serial.print(board_uid);
  if (isLeft) {
    Serial.println(": Controller");
  } else {
    Serial.println(": Extension");
  }

  randomSeed(isLeft * 42);

  load();
}

#endif