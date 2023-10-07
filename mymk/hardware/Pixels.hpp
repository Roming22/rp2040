#ifndef MYMK_HARDWARE_PIXELS
#define MYMK_HARDWARE_PIXELS

#include <Adafruit_NeoPixel.h>

class Pixels : public Adafruit_NeoPixel {
private:
  // Singleton
  Pixels(const unsigned int pin, const unsigned int count)
      : Adafruit_NeoPixel(count, pin) {}

public:
  // Singleton
  Pixels(const Pixels &obj) = delete;
  Pixels &operator=(const Pixels &obj) = delete;
  static Pixels *get(const int pin = 0, const int count = 0) {
    static Pixels *instance = new Pixels(pin, count);
    return instance;
  }

  static void initialize(const unsigned int pin, const unsigned int count) {
    Pixels *_pixels = get(pin, count);
    _pixels->begin();
    _pixels->clear();
    _pixels->show();
    _pixels->setBrightness(255);
    Serial.println("[Pixels] OFF");
  }
};

void set_pixels(const unsigned int pin, const unsigned int count,
                const bool &isLeft) {
  Pixels::initialize(pin, count);
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

#endif