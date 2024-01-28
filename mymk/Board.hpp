#ifndef MYMK_BOARD
#define MYMK_BOARD

void set_board() {
  int len = 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
  char board_uid[len] = "";
  pico_get_unique_board_id_string(board_uid, len);
  std::string left_uid("E461508723193526");
  isLeft = std::string(board_uid) == left_uid;

  // DEBUG_INFO("Checking USB connection");
  pinMode(VUSB_PIN, INPUT);
  delay(10);
  isUsbConnected = digitalRead(VUSB_PIN);

  DEBUG_INFO("%s: %s is %sconnected to USB", board_uid,
             isLeft ? "Controller" : "Extension", isUsbConnected ? "" : "not ");

  randomSeed(isLeft * 42);
}

#endif