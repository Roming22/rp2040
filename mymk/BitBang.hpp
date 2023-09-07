#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#define DEFAULT_INPUT_TIMEOUT_US 2000

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

  inline unsigned int
  receivePulse(unsigned int wait = DEFAULT_INPUT_TIMEOUT_US) const {
    unsigned int duration = 0;

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

    return duration;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 3 / 2;
    unsigned int pulse;
    unsigned int value = 0;

    static unsigned int min = 1000;
    static unsigned int max = 0;

    for (int i = pulses.size() - 1; i >= 0; --i) {
      pulse = pulses.back();
      if (pulse > threshold) {
        bitSet(value, i);
      }
      pulses.pop_back();
      if (pulse > 0) {
        if (pulse < min) {
          min = pulse;
        }
        if (pulse > max) {
          max = pulse;
        }
      }
    }

    return value;
  }

  void sendSync() const {
    bool ready = false;
    std::vector<unsigned int> pulses(1);
    noInterrupts();
    while (!ready) {
      pulses.clear();

      // Send REQUEST
      resetPin();
      sendBit(_active_state);

      // Wait for READY
      inputPin();
      pulses.push_back(receivePulse());
      if (pulses.back() != 0) {
        ready = decodePulses(pulses) != _active_state;
      }
    }
    interrupts();
  }

  void receiveSync() const {
    bool ack = false;
    bool request = false;
    std::vector<unsigned int> pulses(1);

    // Wait for REQUEST
    inputPin();
    noInterrupts();
    while (!request) {
      pulses.clear();
      pulses.push_back(receivePulse());
      if (pulses.back() != 0) {
        request = decodePulses(pulses) == _active_state;
      }
    }
    busy_wait_us_32(_tick + 2);

    // Send READY
    resetPin();
    sendBit(!_active_state);
    interrupts();

    resetPin();
  }

  static void send(const int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    Serial.print("Send value: ");
    Serial.println(value);

    // SYNC
    instance.sendSync();

    // Send bits, LSB first.
    noInterrupts();
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
    std::vector<unsigned int> pulses(length);
    // pulses.reserve(length);

    // SYNC
    instance.receiveSync();

    // Read bits
    noInterrupts();
    instance.inputPin();
    for (int i = length; i > 0; --i) {
      pulses.push_back(instance.receivePulse());
    }
    interrupts();
    instance.resetPin();

    // Decode pulse
    int value = instance.decodePulses(pulses);

    return value;
  }
};

#endif
