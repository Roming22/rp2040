#ifndef MYMK_BITBANG
#define MYMK_BITBANG

namespace hardware {
namespace txrx {

class BitBang {
private:
  unsigned int pin;
  unsigned int msg_len;
  unsigned int input_mode;
  bool active_state;
  unsigned int tick;
  std::vector<unsigned int> pulses;

  // Singleton
  BitBang() {}

public:
  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  static void Setup(const unsigned int i_pin,
                    const unsigned int i_message_ength,
                    const unsigned int i_frequency,
                    const unsigned int i_input_mode = INPUT);

  inline static BitBang &getInstance() {
    static BitBang instance;
    return instance;
  };

  inline void _resetPin() const {
    pinMode(pin, OUTPUT);
    gpio_put(pin, !active_state);
  }

  inline void _inputPin() const { pinMode(pin, input_mode); }

  inline void _sendBit(const bool &bit) const {
    gpio_put(pin, active_state);
    busy_wait_us_32(tick);
    gpio_put(pin, bit);
    busy_wait_us_32(tick);
    busy_wait_us_32(tick);
    gpio_put(pin, !active_state);
    busy_wait_us_32(tick);
  }

  inline unsigned int _receivePulse(unsigned int wait = 1E6) const {
    unsigned int duration = 0;

    // Wait for REST state
    while (gpio_get(pin == active_state)) {
    }

    // Wait for ACTIVE state marking the pulse start
    while (duration == 0 && wait-- > 0) {
      if (gpio_get(pin == active_state)) {
        duration = 1;
      }
      busy_wait_us_32(1);
    }
    // Wait for REST state marking the pulse end
    while (gpio_get(pin == active_state)) {
      ++duration;
      busy_wait_us_32(1);
    }

    return duration;
  }

  inline void _sendData(const unsigned &value,
                        const unsigned int &length) const {
    // Send bits, LSB first.
    for (int i = 0; i < length; ++i) {
      _sendBit(bitRead(value, i));
    }
  }

  inline void _receiveData(const unsigned int &length) {
    // Read bits
    _inputPin();
    for (int i = length; i > 0; --i) {
      pulses.push_back(_receivePulse(1E6));
    }
    _resetPin();
  }

  inline void _sendSyncSignal() {
    while (true) {
      pulses.clear();

      // Send REQUEST
      _resetPin();
      _sendBit(active_state);

      // Wait for READY
      _inputPin();
      pulses.push_back(_receivePulse());
      if (pulses.back() != 0) {
        if (_decodePulses() != active_state) {
          _resetPin();
          return;
        }
      }
    }
  }

  inline void _receiveSyncSignal() {
    // Wait for REQUEST
    _inputPin();
    while (true) {
      pulses.clear();
      pulses.push_back(_receivePulse());
      if (pulses.back() != 0 and _decodePulses() == active_state) {
        break;
      }
    }
    busy_wait_us_32(tick + 12);

    // Send READY
    _resetPin();
    _sendBit(!active_state);
  }

  unsigned int _decodePulses();

  static void SendData(const unsigned int &value);
  static unsigned int ReceiveData();
  static void SendSyncSignal();
  static void ReceiveSyncSignal();
  static void Send(const int &value);
  static unsigned int Receive();
};
} // namespace txrx
} // namespace hardware

#endif