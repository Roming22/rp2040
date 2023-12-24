#include "Pixels.h"

#include "../../logic/ObjectManager.h"
#include "../../utils/Debug.hpp"

namespace hardware {
namespace led {
Pixels::Ptr Pixels::instance = nullptr;

Pixels::Ptr Pixels::New(const unsigned int pin, const unsigned int count) {
  logic::ObjectManager::Register("hardware::led::Pixels");
  return Ptr(new Pixels(pin, count));
}

Pixels::ColorPtr Pixels::NewColorPtr(int red, int green, int blue, int white) {
  ColorPtr color = ColorPtr(new std::vector<int>());
  color->push_back(red);
  color->push_back(green);
  color->push_back(blue);
  color->push_back(white);
  return color;
}

void Pixels::Setup(const unsigned int pin, const unsigned int count) {
  instance = New(pin, count);
  Adafruit_NeoPixel &pixels = instance->pixels;
  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels.setBrightness(255);
  DEBUG_DEBUG("[Pixels] OFF");
}

void Pixels::Setup(const unsigned int pin, const unsigned int count,
                   const bool &isLeft) {
  Pixels::Setup(pin, count);
  Pixels::Set(0, 0, 0, 0);
  delay(200);
  Pixels::Set(0, 255 * isLeft, 255 * (1 - isLeft), 0);
  delay(1000);
  Pixels::Set(0, 0, 0, 0);
}

void Pixels::Set(const unsigned int address, const unsigned int red,
                 const unsigned int green, const unsigned int blue) {
  Adafruit_NeoPixel &pixels = instance->pixels;
  if (address == 0) {
    pixels.fill(pixels.Color(red, green, blue));
  } else {
    pixels.setPixelColor(address - 1, red, green, blue);
  }
  while (!pixels.canShow()) {
  };
  pixels.show();
}

unsigned int Pixels::FlipFlop() {
  static int color = 0;
  int address = 0;
  int red = 255 * (color == 0 || color == 1 || color == 5);
  int green = 255 * (color == 1 || color == 2 || color == 3);
  int blue = 255 * (color == 3 || color == 4 || color == 5);
  color = ++color % 6;

  Pixels::Set(address, red, green, blue);
  unsigned int data = (address << 24) + (red << 16) + (green << 8) + blue;
  return data;
}
} // namespace led
} // namespace hardware