#include "BitBang.h"
#include "../../utils/Debug.hpp"

#include <DIO2.h>
namespace hardware {
namespace txrx {
inline BitBang &BitBang::getInstance() {
  static BitBang instance;
  return instance;
}

void BitBang::Setup(const unsigned int pin, const unsigned int message_length,
                    const unsigned int frequency, const unsigned input_mode) {
  BitBang &instance = getInstance();
  instance._pin = pin;
  instance.msg_len = message_length;
  instance._input_mode = input_mode;
  instance._active_state = (instance._input_mode);
  instance._resetPin();

  instance._tick = 1E6 / frequency / 4;

  DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
             instance._active_state);
  DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d", instance._tick,
             int(1E6 / (instance._tick * 4)));
}

inline void BitBang::_resetPin() const {
  pinMode2(_pin, OUTPUT);
  digitalWrite2(_pin, !_active_state);
}
inline void BitBang::_inputPin() const { pinMode(_pin, _input_mode); }

inline void BitBang::_sendBit(const bool &bit) const {
  digitalWrite2(_pin, _active_state);
  busy_wait_us_32(_tick);
  digitalWrite2(_pin, bit);
  busy_wait_us_32(_tick);
  busy_wait_us_32(_tick);
  digitalWrite2(_pin, !_active_state);
  busy_wait_us_32(_tick);
}

inline unsigned int BitBang::_receivePulse(unsigned int wait) const {
  unsigned int duration = 0;

  // Wait for REST state
  while (digitalRead2(_pin) == _active_state) {
  }

  // Wait for ACTIVE state marking the pulse start
  while (duration == 0 && wait-- > 0) {
    if (digitalRead2(_pin) == _active_state) {
      duration = 1;
    }
    busy_wait_us_32(1);
  }
  // Wait for REST state marking the pulse end
  while (digitalRead2(_pin) == _active_state) {
    ++duration;
    busy_wait_us_32(1);
  }

  return duration;
}

unsigned int BitBang::_decodePulses() {
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

inline void BitBang::_sendSync() {
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

inline void BitBang::_receiveSync() {
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

void BitBang::SendSync() {
  static BitBang &instance = getInstance();

  noInterrupts();
  instance._sendSync();
  interrupts();
}

void BitBang::ReceiveSync() {
  static BitBang &instance = getInstance();

  noInterrupts();
  instance._receiveSync();
  interrupts();
}

inline void BitBang::_sendData(const unsigned &value) const {
  // Send bits, LSB first.
  for (int i = 0; i < msg_len; ++i) {
    _sendBit(bitRead(value, i));
  }
}

inline void BitBang::_receiveData() {
  // Read bits
  _inputPin();
  for (int i = msg_len; i > 0; --i) {
    _pulses.push_back(_receivePulse(1E6));
  }
  _resetPin();
}

void BitBang::SendData(const unsigned int &value) {
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

unsigned int BitBang::ReceiveData() {
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

void BitBang::Send(const int &value) {
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

unsigned int BitBang::Receive() {
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

void BitBang::Out(const unsigned int value) { getInstance().out.push(value); }

std::queue<unsigned int> &BitBang::In() { return getInstance().in; }

void BitBang::Tick() {
  BitBang &instance = getInstance();
  int data = 0;

  data = BitBang::Receive();
  instance.in.push(data);

  if (instance.out.size() != 0) {
    data = instance.out.front();
    instance.out.pop();
  }
  BitBang::Send(data);
}
} // namespace txrx
} // namespace hardware