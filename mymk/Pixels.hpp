#ifndef MYMK_PIXELS
#define MYMK_PIXELS

#include <Adafruit_NeoPixel.h>

class Pixels: public Adafruit_NeoPixel {
    private:
    // Singleton
    Pixels(const int pin, const int count) : Adafruit_NeoPixel(count, pin) {}

    public:
    // Singleton
    Pixels(const Pixels& obj) = delete;
    Pixels& operator=(const Pixels& obj) = delete;
    static Pixels* get(const int pin = 0, const int count = 0) {
        static Pixels* instance = new Pixels(pin, count);
        return instance;
    }
    
    static void initialize(const int pin, const int count) {
        Pixels* _pixels = get(pin, count);
        _pixels->begin();
        _pixels->clear();
        _pixels->show();
        _pixels->setBrightness(255);
        Serial.println("[Pixels] OFF");
    }
};

#endif