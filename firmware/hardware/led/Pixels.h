#ifndef MYMK_HARDWARE_LED_PIXELS
#define MYMK_HARDWARE_LED_PIXELS

#include <Adafruit_NeoPixel.h>
#include <vector>

namespace hardware {
namespace led {
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

  static void Setup(const unsigned int pin, const unsigned int count);
  static void Setup(const unsigned int pin, const unsigned int count,
                    const bool &isLeft);

  static void Set(const unsigned int address, const unsigned int red,
                  const unsigned int green, const unsigned int blue);

  static unsigned int FlipFlop();
};
} // namespace led
} // namespace hardware
#endif