#include <string>

#include "mymk/mymk.hpp"

#define DATA_PIN 1
#define PIXELS_PIN 0
#define PIXELS_COUNT 4

bool isLeft = 0;

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
}

void set_pixels() {
  Pixels::initialize(PIXELS_PIN, PIXELS_COUNT);
  Pixels &pixels = *Pixels::get();

  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
  delay(200);
  pixels.fill(pixels.Color(255 * isLeft, 255 * (1 - isLeft), 0));
  pixels.show();
  delay(1000);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
}

void set_bitbang() { BitBang::initialize(DATA_PIN, 1 << 13); }

void setup() {
  Serial.begin(9600);
  delay(1300);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[ECHO BLINK]");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");

  set_board();
  set_pixels();
  set_bitbang();

  Serial.println("");
  Serial.println("# Looping");
}

void blinkLeds() {
  static int color = 0;
  static Pixels &pixels = *Pixels::get();

  int red = 255 * (color == 0 || color == 1 || color == 5);
  int green = 255 * (color == 1 || color == 2 || color == 3);
  int blue = 255 * (color == 3 || color == 4 || color == 5);

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
  delay(25);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
  delay(25);

  color = ++color % 6;
}

void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 1;
  static unsigned int badValues = 0;
  Pixels &pixels = *Pixels::get();

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
  } else {
    Serial.println(" [Extension]");
  }

  // Receive GO
  if (!isLeft || loopIndex > 1) {
    Serial.println("GET");
    msg = BitBang::receive(msgLen) + isLeft;
  }

  if (loopIndex != msg) {
    Serial.println("");
    Serial.println("");
    Serial.print("!!! Bad value: ");
    Serial.print(++badValues);
    Serial.println(" !!!");
    delay(2000);
  }
  Serial.println("");
  Serial.println("BLINK");
  if (loopIndex % 500 == 0) {
    Serial.print("Bad value check: ");
    Serial.print(badValues);
    pixels.fill(pixels.Color(255 * (badValues > 0), 255 * (badValues == 0), 0));
    pixels.show();
    delay(2000);
  }
  blinkLeds();

  // Send GO
  Serial.println("");
  Serial.println("POST");
  BitBang::send(loopIndex, msgLen);
}