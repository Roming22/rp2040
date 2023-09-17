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
    instance._resetPin();

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

  inline void _resetPin() const {
    pinMode(_pin, OUTPUT);
    gpio_put(_pin, !_active_state);
  }

  inline void _inputPin() const { pinMode(_pin, _input_mode); }

  inline void _sendBit(const bool &bit) const {
    gpio_put(_pin, _active_state);
    busy_wait_us_32(_tick);
    gpio_put(_pin, bit);
    busy_wait_us_32(_tick);
    busy_wait_us_32(_tick);
    gpio_put(_pin, !_active_state);
    busy_wait_us_32(_tick);
  }

  inline unsigned int
  _receivePulse(unsigned int wait = DEFAULT_INPUT_TIMEOUT_US) const {
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

  unsigned int _decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 3 / 2;
    unsigned int pulse;
    unsigned int value = 0;

    for (int i = pulses.size() - 1; i >= 0; --i) {
      pulse = pulses.back();
      if (pulse > threshold) {
        bitSet(value, i);
      }
      pulses.pop_back();
    }

    return value;
  }

  inline void _sendSync() const {
    std::vector<unsigned int> pulses(1);
    while (true) {
      pulses.clear();

      // Send REQUEST
      _resetPin();
      _sendBit(_active_state);

      // Wait for READY
      _inputPin();
      pulses.push_back(_receivePulse());
      if (pulses.back() != 0) {
        if (_decodePulses(pulses) != _active_state) {
          _resetPin();
          return;
        }
      }
    }
  }

  inline void _receiveSync() const {
    bool ack = false;
    bool request = false;
    std::vector<unsigned int> pulses(1);

    // Wait for REQUEST
    _inputPin();
    while (!request) {
      pulses.clear();
      pulses.push_back(_receivePulse());
      if (pulses.back() != 0) {
        request = _decodePulses(pulses) == _active_state;
      }
    }
    busy_wait_us_32(_tick + 12);

    // Send READY
    _resetPin();
    _sendBit(!_active_state);
  }

  static void sendSync() {
    static const BitBang &instance = getInstance();

    noInterrupts();
    instance._sendSync();
    interrupts();
  }

  static void receiveSync() {
    static const BitBang &instance = getInstance();

    noInterrupts();
    instance._receiveSync();
    interrupts();
  }

  inline void _sendData(const unsigned &value,
                        const unsigned int &length) const {
    // Send bits, LSB first.
    for (int i = 0; i < length; ++i) {
      _sendBit(bitRead(value, i));
    }
  }

  inline void _receiveData(std::vector<unsigned int> &pulses,
                           const unsigned int &length) const {
    // Read bits
    _inputPin();
    for (int i = length; i > 0; --i) {
      pulses.push_back(_receivePulse(1E6));
    }
    _resetPin();
  }

  static void sendData(const unsigned int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    Serial.print("Send value: ");
    Serial.println(value);

    // Send GO
    noInterrupts();
    instance._sendBit(!instance._active_state);

    // Send bits, LSB first.
    instance._sendData(value, length);
    interrupts();

    Serial.print("Sent bits: ");
    Serial.print(value, BIN);
    Serial.println();
  }

  static unsigned int receiveData(const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(length);
    unsigned int value;

    noInterrupts();
    instance._inputPin();

    // Wait for GO
    while (instance._receivePulse() == 0) {
    }

    // Read bits
    instance._receiveData(pulses, length);
    interrupts();

    // Decode pulse
    value = instance._decodePulses(pulses);

    Serial.print("Received bits: ");
    Serial.println(value, BIN);
    Serial.print("Received value: ");
    Serial.println(value);

    return value;
  }

  static void send(const int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    Serial.print("Send value: ");
    Serial.println(value);

    noInterrupts();
    instance._sendSync();
    busy_wait_us_32(instance._tick + 12);
    instance._sendData(value, length);
    interrupts();

    Serial.print("Sent bits: ");
    Serial.print(value, BIN);
    Serial.println();
  }

  static unsigned int receive(const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(length);
    unsigned int value;

    noInterrupts();
    instance._receiveSync();
    instance._inputPin();
    instance._receiveData(pulses, length);
    interrupts();

    // Decode pulse
    value = instance._decodePulses(pulses);

    Serial.print("Received bits: ");
    Serial.println(value, BIN);
    Serial.print("Received value: ");
    Serial.println(value);

    return value;
  }
};

void set_bitbang() { BitBang::initialize(DATA_PIN, 1 << 13); }

#endif
