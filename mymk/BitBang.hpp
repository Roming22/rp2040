#ifndef MYMK_BITBANG
#define MYMK_BITBANG

class BitBang {
    private:
    unsigned int _pin;
    unsigned int _micro_tick;
    unsigned int _tick;

    // Singleton
    BitBang() {}

    public:
    // Singleton
    BitBang(const BitBang& obj) = delete;
    BitBang& operator=(const BitBang& obj) = delete;

    static BitBang& getInstance() {
        static BitBang instance;
        return instance;
    }
    
    static void initialize(const int pin, const int frequency) {
        BitBang& instance = getInstance();
        instance._pin = pin;
        // Reset pin
        pinMode(instance._pin, OUTPUT);
        digitalWrite(instance._pin, LOW);

        instance._micro_tick = 30;
        instance._tick = 1E6 / frequency - instance._micro_tick * 2;

        Serial.print("BitBang tick (usec): ");
        Serial.print(instance._tick);
        Serial.print("    Frequency (Hz): ");
        Serial.println(int(1E6 / (instance._tick + instance._micro_tick * 2)));
    }

    inline static void tick(const BitBang& instance) {
        delayMicroseconds(instance._tick);
    }

    inline static void micro_tick(const BitBang& instance) {
        delayMicroseconds(instance._micro_tick);
    }

    inline static void sendBit(const bool& bit, const BitBang& instance) {
        digitalWrite(instance._pin, HIGH);
        micro_tick(instance);
        digitalWrite(instance._pin, bit);
        tick(instance);
        digitalWrite(instance._pin, LOW);
        micro_tick(instance);
    }

    static void send() {
        static const BitBang& instance = getInstance();
        Serial.print("Send value: ");
        Serial.println(0);
        Serial.print("Send bits: ");
        Serial.print(0, BIN);
        Serial.println();
    
        // Send bits, LSB first.
        sendBit(false, instance);
    }

    static unsigned int receive() {
        static const BitBang& instance = getInstance();
        static int min = 1000;
        static int max = 0;
        // Set pin
        pinMode(instance._pin, INPUT);

        // Read bits
        int value = pulseIn(instance._pin, HIGH);

        // Reset pin
        pinMode(instance._pin, OUTPUT);
        digitalWrite(instance._pin, LOW);

        if (value > 0 && value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }

        if (value > 0) {
            Serial.print("Received pulse length: ");
            Serial.print(value);
            Serial.print(" (");
            Serial.print(min);
            Serial.print(",");
            Serial.print(max);
            Serial.println(")");
        }

        return value;
    }
};

#endif