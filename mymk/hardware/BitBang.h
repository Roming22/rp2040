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

  inline static BitBang &getInstance();

  static void Setup(const unsigned int pin, const unsigned int message_length,
                    const unsigned int frequency,
                    const unsigned input_mode = INPUT);

  inline void _resetPin() const;

  inline void _inputPin() const;

  inline void _sendBit(const bool &bit) const;

  inline unsigned int
  _receivePulse(unsigned int wait = DEFAULT_INPUT_TIMEOUT_US) const;

  unsigned int _decodePulses();

  inline void _sendSync();

  inline void _receiveSync();

  static void SendSync();

  static void ReceiveSync();

  inline void _sendData(const unsigned &value) const;

  inline void _receiveData();

  static void SendData(const unsigned int &value);

  static unsigned int ReceiveData();

  static void Send(const int &value);

  static unsigned int Receive();

  static void Out(const unsigned int value);

  static std::queue<unsigned int> &In();

  static void Tick();
};
#endif