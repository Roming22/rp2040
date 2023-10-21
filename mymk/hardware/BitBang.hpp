// Max frequency on an RP2040: 31250Hz (8us tick)

#ifndef MYMK_HARDWARE_BITBANG
#define MYMK_HARDWARE_BITBANG

#include <queue>
#define DEFAULT_INPUT_TIMEOUT_US 2000

class BitBang {
private:
  unsigned int _pin;
  unsigned int msg_len;
  unsigned int _input_mode;
  bool _active_state;
  unsigned int _tick;
  std::vector<unsigned int> _pulses;

  std::queue<unsigned int> in, out;

  // Singleton
  BitBang() {}

public:
  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  inline static BitBang &getInstance() {
    static BitBang instance;
    return instance;
  }

  static void initialize(const unsigned int pin,
                         const unsigned int message_length,
                         const unsigned int frequency,
                         const unsigned input_mode = INPUT) {
    BitBang &instance = getInstance();
    instance._pin = pin;
    instance.msg_len = message_length;
    instance._input_mode = input_mode;
    instance._active_state = (instance._input_mode == INPUT);
    instance._resetPin();

    instance._tick = 1E6 / frequency / 4;

    DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
               instance._active_state);
    DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d", instance._tick,
               int(1E6 / (instance._tick * 4)));
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

  unsigned int _decodePulses() {
    static unsigned int threshold = _tick * 3 / 2;
    unsigned int pulse;
    unsigned int value = 0;

    for (int i = _pulses.size() - 1; i >= 0; --i) {
      pulse = _pulses.back();
      if (pulse > threshold) {
        bitSet(value, i);
      }
      _pulses.pop_back();
    }

    return value;
  }

  inline void _sendSync() {
    while (true) {
      _pulses.clear();

      // Send REQUEST
      _resetPin();
      _sendBit(_active_state);

      // Wait for READY
      _inputPin();
      _pulses.push_back(_receivePulse());
      if (_pulses.back() != 0) {
        if (_decodePulses() != _active_state) {
          _resetPin();
          return;
        }
      }
    }
  }

  inline void _receiveSync() {
    // Wait for REQUEST
    _inputPin();
    while (true) {
      _pulses.clear();
      _pulses.push_back(_receivePulse());
      if (_pulses.back() != 0 and _decodePulses() == _active_state) {
        break;
      }
    }
    busy_wait_us_32(_tick + 12);

    // Send READY
    _resetPin();
    _sendBit(!_active_state);
  }

  static void SendSync() {
    static BitBang &instance = getInstance();

    noInterrupts();
    instance._sendSync();
    interrupts();
  }

  static void ReceiveSync() {
    static BitBang &instance = getInstance();

    noInterrupts();
    instance._receiveSync();
    interrupts();
  }

  inline void _sendData(const unsigned &value) const {
    // Send bits, LSB first.
    for (int i = 0; i < msg_len; ++i) {
      _sendBit(bitRead(value, i));
    }
  }

  inline void _receiveData() {
    // Read bits
    _inputPin();
    for (int i = msg_len; i > 0; --i) {
      _pulses.push_back(_receivePulse(1E6));
    }
    _resetPin();
  }

  static void SendData(const unsigned int &value) {
    static BitBang &instance = getInstance();
    DEBUG_DEBUG("Send value: %d", value);

    // Send GO
    noInterrupts();
    instance._sendBit(!instance._active_state);

    // Send bits, LSB first.
    instance._sendData(value);
    interrupts();

    // Serial.print("Sent bits: ");
    // Serial.println(value, BIN);
  }

  static unsigned int ReceiveData() {
    static BitBang &instance = getInstance();
    instance._pulses.reserve(instance.msg_len);
    static unsigned int value;

    noInterrupts();
    instance._inputPin();

    // Wait for GO
    while (instance._receivePulse() == 0) {
    }

    // Read bits
    instance._receiveData();
    interrupts();

    // Decode pulse
    value = instance._decodePulses();

    // Serial.print("Received bits: ");
    // Serial.println(value, BIN);
    DEBUG_DEBUG("Received value: %d", value);

    return value;
  }

  static void Send(const int &value) {
    static BitBang &instance = getInstance();
    DEBUG_DEBUG("Send value: %d", value);

    noInterrupts();
    instance._sendSync();
    busy_wait_us_32(instance._tick * 4);
    instance._sendData(value);
    interrupts();

    // Serial.print("Sent bits: ");
    // Serial.println(value, BIN);
  }

  static unsigned int Receive() {
    static BitBang &instance = getInstance();
    instance._pulses.reserve(instance.msg_len);
    static unsigned int value;

    noInterrupts();
    instance._receiveSync();
    instance._receiveData();
    interrupts();

    // Decode pulse
    value = instance._decodePulses();

    // Serial.print("Received bits: ");
    // Serial.println(value, BIN);
    DEBUG_DEBUG("Received value: %d", value);

    return value;
  }

  static void Out(const unsigned int value) { getInstance().out.push(value); }

  static std::queue<unsigned int> &In() { return getInstance().in; }

  static void Tick() {
    BitBang &instance = getInstance();
    int data = 0;

    data = Receive();
    instance.in.push(data);

    if (instance.out.size() != 0) {
      data = instance.out.front();
      instance.out.pop();
    }
    Send(data);
  }
};
#endif