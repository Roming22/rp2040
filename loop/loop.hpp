void blinkLeds(unsigned int duration) {
  static int color = 0;
  static Pixels &pixels = *Pixels::get();

  int red = 255 * (color == 0 || color == 1 || color == 5);
  int green = 255 * (color == 1 || color == 2 || color == 3);
  int blue = 255 * (color == 3 || color == 4 || color == 5);

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
  delay(duration);
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

#include "1_relay.hpp"
#include "2_echo.hpp"
#include "3_sync.hpp"
#include "4_data.hpp"
#include "5_keymatrix.hpp"
