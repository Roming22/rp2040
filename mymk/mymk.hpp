#include "BitBang.hpp"
#include "Pixels.hpp"

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
    delay(1000); // Ensures that it starts the loop last.
  }
}
void setup() {
  Serial.begin(9600);
  delay(1300);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[ECHO-SYNC BLINK]");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");

  set_board();
  set_pixels();
  set_bitbang();

  Serial.println("");
  Serial.println("# Looping");
}