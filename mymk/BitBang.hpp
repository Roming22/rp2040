#ifndef MYMK_BITBANG
#define MYMK_BITBANG

class BitBang {
private:
  unsigned int _pin;
  unsigned int _input_mode;
  bool _active_state;
  unsigned int _tick;

  // Singleton
  BitBang() {}

public:
  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  static BitBang &getInstance() {
    static BitBang instance;
    return instance;
  }

  static void initialize(const unsigned int pin, const unsigned int frequency,
                         const unsigned input_mode = INPUT) {
    BitBang &instance = getInstance();
    instance._pin = pin;
    instance._input_mode = input_mode;
    instance._active_state = (instance._input_mode == INPUT);
    instance.resetPin();

    instance._tick = 1E6 / frequency / 4;

    Serial.print("LOW: ");
    Serial.print(LOW);
    Serial.print("    HIGH: ");
    Serial.print(HIGH);
    Serial.print("    ACTIVE: ");
    Serial.println(instance._active_state);

    Serial.print("BitBang tick (usec): ");
    Serial.print(instance._tick);
    Serial.print("    Frequency (Hz): ");
    Serial.println(int(1E6 / (instance._tick * 4)));
  }

  inline void resetPin() const {
    pinMode(_pin, OUTPUT);
    gpio_put(_pin, !_active_state);
  }

  inline void inputPin() const { pinMode(_pin, _input_mode); }

  inline void sendBit(const bool &bit) const {
    gpio_put(_pin, _active_state);
    busy_wait_us_32(_tick);
    gpio_put(_pin, bit);
    busy_wait_us_32(_tick);
    busy_wait_us_32(_tick);
    gpio_put(_pin, !_active_state);
    busy_wait_us_32(_tick);
  }

  inline unsigned int receivePulse(unsigned int wait = 1E6) const {
    unsigned int duration = 0;

    static int min = 1000;
    static int max = 0;

    // Wait for REST state
    while (gpio_get(_pin) == _active_state) {
    }

    // Wait for ACTIVE state marking the pulse start
    while (duration == 0 && wait-- > 0) {
      if (gpio_get(_pin) == _active_state) {
        duration = 1;
      }
      busy_wait_us_32(1);
    }
    // Wait for REST state marking the pulse end
    while (gpio_get(_pin) == _active_state) {
      ++duration;
      busy_wait_us_32(1);
    }
    if (duration > 0) {
      if (duration < min) {
        min = duration;
      }
      if (duration > max) {
        max = duration;
      }
      Serial.print("Received pulse length: ");
      Serial.print(duration);
      Serial.print(" (");
      Serial.print(min);
      Serial.print(",");
      Serial.print(max);
      Serial.println(")");
    }

    return duration;
  }

  static void send() {
    static const BitBang &instance = getInstance();
    Serial.print("Send value: ");
    Serial.println(0);

    // Send bits, LSB first.
    noInterrupts();
    instance.sendBit(!instance._active_state);
    interrupts();

    Serial.print("Sent bits: ");
    Serial.print(0, BIN);
    Serial.println();
  }

  static unsigned int receive() {
    static const BitBang &instance = getInstance();
    // Set pin
    instance.inputPin();

    // Read bits
    int value = instance.receivePulse();

    // Reset pin
    instance.resetPin();

    return value;
  }
};

#endif
