#ifndef MYMK_HARDWARE_LED_PIXELS
#define MYMK_HARDWARE_LED_PIXELS

#include <Adafruit_NeoPixel.h>
#include <memory>
#include <vector>

namespace hardware {
namespace led {
class Pixels : public Adafruit_NeoPixel {
private:
  typedef std::shared_ptr<Pixels> Ptr;
  static Pixels::Ptr instance;
  Adafruit_NeoPixel pixels;
  // Singleton
  Pixels(const unsigned int pin, const unsigned int count)
      : pixels(count, pin) {}

public:
  typedef std::shared_ptr<std::vector<int>> ColorPtr;
  static ColorPtr NewColorPtr(int red, int green, int blue, int white);

  // Singleton
  Pixels(const Pixels &obj) = delete;
  Pixels &operator=(const Pixels &obj) = delete;

  static Ptr New(const unsigned int pin, const unsigned int count);
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