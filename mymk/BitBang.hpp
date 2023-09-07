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

    // Wait for REST state
    while (gpio_get(_pin == _active_state)) {
    }

    // Wait for ACTIVE state marking the pulse start
    while (duration == 0 && wait-- > 0) {
      if (gpio_get(_pin == _active_state)) {
        duration = 1;
      }
      busy_wait_us_32(1);
    }
    // Wait for REST state marking the pulse end
    while (gpio_get(_pin == _active_state)) {
      ++duration;
      busy_wait_us_32(1);
    }

    return duration;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 3 / 2;
    unsigned int pulse;
    unsigned int value = 0;

    static unsigned int min = 1000;
    static unsigned int max = 0;

    Serial.print("Pulses: ");
    for (int i = pulses.size() - 1; i >= 0; --i) {
      pulse = pulses.back();
      if (pulse > threshold) {
        bitSet(value, i);
      }
      pulses.pop_back();
      Serial.print(pulse);
      Serial.print(", ");
      if (pulse > 0) {
        if (pulse < min) {
          min = pulse;
        }
        if (pulse > max) {
          max = pulse;
        }
      }
    }
    Serial.print(" (");
    Serial.print(min);
    Serial.print(",");
    Serial.print(max);
    Serial.println(")");

    return value;
  }

  static void send(const unsigned int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    Serial.print("Send value: ");
    Serial.println(value);

    // Send GO
    noInterrupts();
    instance.sendBit(!instance._active_state);

    // Send bits, LSB first.
    for (int i = 0; i < length; ++i) {
      instance.sendBit(bitRead(value, i));
    }
    interrupts();

    Serial.print("Sent bits: ");
    Serial.print(value, BIN);
    Serial.println();
  }

  static unsigned int receive(const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(length);

    instance.inputPin();

    // Wait for GO
    while (instance.receivePulse() == 0) {
    }

    // Read bits
    for (int i = length; i > 0; --i) {
      pulses.push_back(instance.receivePulse());
    }
    instance.resetPin();

    // Decode pulse
    int value = instance.decodePulses(pulses);

    if (value > 0) {
      Serial.print("Received bits: ");
      Serial.println(value, BIN);
      Serial.print("Received value: ");
      Serial.println(value);
    }

    return value;
  }
};

#endif
