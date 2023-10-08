#ifndef MYMK_HARDWARE_LED_PIXELS
#define MYMK_HARDWARE_LED_PIXELS

#include <Adafruit_NeoPixel.h>
#include <vector>

class Pixels : public Adafruit_NeoPixel {
private:
  static Pixels *instance;
  Adafruit_NeoPixel pixels;
  // Singleton
  Pixels(const unsigned int pin, const unsigned int count)
      : pixels(count, pin) {}

public:
  // Singleton
  Pixels(const Pixels &obj) = delete;
  Pixels &operator=(const Pixels &obj) = delete;
  static Pixels *Setup(const int pin = 0, const int count = 0) {
    static Pixels *instance = new Pixels(pin, count);
    return instance;
  }

  static void Setup(const unsigned int pin, const unsigned int count) {
    instance = new Pixels(pin, count);
    Adafruit_NeoPixel &pixels = instance->pixels;
    pixels.begin();
    pixels.clear();
    pixels.show();
    pixels.setBrightness(255);
    Serial.println("[Pixels] OFF");
  }

  static void Set(const unsigned int address, const unsigned int red,
                  const unsigned int green, const unsigned int blue) {
    Adafruit_NeoPixel &pixels = instance->pixels;
    if (address == 0) {
      pixels.fill(pixels.Color(red, green, blue));
    }
    pixels.show();
  }

  static void FlipFlop(std::vector<unsigned int> &messages) {
    static int color = 0;
    int address = 0;
    int red = 255 * (color == 0 || color == 1 || color == 5);
    int green = 255 * (color == 1 || color == 2 || color == 3);
    int blue = 255 * (color == 3 || color == 4 || color == 5);
    color = ++color % 6;

    Pixels::Set(address, red, green, blue);
    unsigned int data = (address << 24) + (red << 16) + (green << 8) + blue;
    messages.push_back(data);
  }
};
Pixels *Pixels::instance = nullptr;

void set_pixels(const unsigned int pin, const unsigned int count,
                const bool &isLeft) {
  Pixels::Setup(pin, count);
  Pixels::Set(0, 0, 0, 0);
  delay(200);
  Pixels::Set(0, 255 * isLeft, 255 * (1 - isLeft), 0);
  delay(1000);
  Pixels::Set(0, 0, 0, 0);
}

void error(unsigned int duration) {
  Pixels::Set(0, 255, 255, 255);
  delay(duration);
  Pixels::Set(0, 0, 0, 0);
}

#endif