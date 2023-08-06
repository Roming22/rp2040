#ifndef MYMK_BITBANG
#define MYMK_BITBANG

class BitBang {
    private:
    int _pin;
    int _tick;

    // Singleton
    BitBang() {}

    public:
    // Singleton
    BitBang(const BitBang& obj) = delete;
    BitBang& operator=(const BitBang& obj) = delete;
    static BitBang& get() {
        static BitBang instance;
        return instance;
    }
    
    static void initialize(const int pin, const int frequency) {
        BitBang& instance = get();
        instance._pin = pin;
        instance._tick = 1E6 / frequency;
        Serial.print("BitBang tick (usec): ");
        Serial.println(instance._tick);
        Serial.print("Frequency (Hz): ");
        Serial.println(int(1E6 / instance._tick));
    }
};

#endif