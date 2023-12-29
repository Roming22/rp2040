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

void blinkLeds() {
  static int color = 0;
  static Pixels &pixels = *Pixels::get();

  int red = 255 * (color == 0 || color == 1 || color == 5);
  int green = 255 * (color == 1 || color == 2 || color == 3);
  int blue = 255 * (color == 3 || color == 4 || color == 5);

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
  delay(25);
  // pixels.fill(pixels.Color(0, 0, 0));
  // pixels.show();
  // delay(25);

  color = ++color % 6;
}

#endif