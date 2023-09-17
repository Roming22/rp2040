#ifndef MYMK_PIXELS
#define MYMK_PIXELS

#include <Adafruit_NeoPixel.h>

class Pixels : public Adafruit_NeoPixel {
private:
  // Singleton
  Pixels(const int pin, const int count) : Adafruit_NeoPixel(count, pin) {}

public:
  // Singleton
  Pixels(const Pixels &obj) = delete;
  Pixels &operator=(const Pixels &obj) = delete;
  static Pixels *get(const int pin = 0, const int count = 0) {
    static Pixels *instance = new Pixels(pin, count);
    return instance;
  }

  static void initialize(const int pin, const int count) {
    Pixels *_pixels = get(pin, count);
    _pixels->begin();
    _pixels->clear();
    _pixels->show();
    _pixels->setBrightness(255);
    Serial.println("[Pixels] OFF");
  }
};

void set_pixels(const bool &isLeft) {
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

#endif