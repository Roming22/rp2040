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

  randomSeed(isLeft * 42);
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
  delay(5000);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[BITBANG]");
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
  delay(3000);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
  color = ++color % 6;
}

void error(unsigned int duration) {
  static Pixels &pixels = *Pixels::get();

  pixels.fill(pixels.Color(255, 255, 255));
  pixels.show();
  delay(duration);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
}

void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);

  if (isLeft) {
    Serial.println(" [Controller]");
    Serial.println("POST Value");
    BitBang::send(loopIndex, msgLen);
  } else {
    Serial.println(" [Extension]");
    Serial.println("GET Value");
    msg = BitBang::receive(msgLen);
  }

  if (loopIndex % 5000 == 42) {
    blinkLeds();
    Serial.print("Error count: ");
    Serial.println(inError);
    if (inError) {
      error(5000);
    }
  }

  // Send GO
  Serial.println("");
  if (isLeft) {
    Serial.println("GET Value");
    msg = BitBang::receive(msgLen);
  } else {
    Serial.println("POST Value");
    BitBang::send(loopIndex, msgLen);
  }

  if (msg != loopIndex) {
    ++inError;
    Serial.println("");
    Serial.println("");
    Serial.println("!!! Bad value !!!");
    Serial.print(msg);
    Serial.print("!=");
    Serial.println(loopIndex);
    Serial.print("Error count: ");
    Serial.println(inError);
    error(10000);
    if (!isLeft) {
      loopIndex = msg;
    }
  }
}
