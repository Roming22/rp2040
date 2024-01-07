#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#include "../../utils/Debug.hpp"

#include <cmath>
#include <list>

namespace hardware {
namespace txrx {

class BitBang {
  unsigned int _pin;
  unsigned int msgLength;
  bool _active_state;
  bool connection;
  unsigned int _tick;
  std::list<unsigned int> values;

  // Singleton
  BitBang() {}

  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  static BitBang &GetInstance() {
    static BitBang instance;
    return instance;
  }

  inline void wait_until(unsigned long time) const {
    while (micros() < time) {
    }
  }

  inline void openChannel() const {
    digitalWrite(_pin, !_active_state);
    pinMode(_pin, INPUT_PULLUP);
  }

  inline void closeChannel() const {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _active_state);
  }

  inline bool isChannelOpened() const {
    bool quiet = false;
    openChannel();
    while (!quiet) {
      while (!quiet) {
        quiet = digitalRead(_pin) != _active_state;
      }
      unsigned long timeout = micros() + _tick * 8;
      while (quiet && micros() < timeout) {
        quiet = digitalRead(_pin) != _active_state;
      }
    }
    closeChannel();
    return quiet;
  }

  inline bool isChannelClosed() const {
    bool closed = false;
    openChannel();
    while (!closed) {
      while (!closed) {
        closed = digitalRead(_pin) == _active_state;
      }
      unsigned long timeout = micros() + _tick * 8;
      while (closed && micros() < timeout) {
        closed = digitalRead(_pin) == _active_state;
      }
    }
    closeChannel();
    return closed;
  }

  inline void sendBit(const bool &bit) const {
    unsigned long time;
    time = micros() + (_tick * (bit ? 3 : 1));
    digitalWrite(_pin, _active_state);
    wait_until(time);
    time = micros() + (_tick * (bit ? 1 : 3));
    digitalWrite(_pin, !_active_state);
    wait_until(time);
  }

  inline unsigned int receivePulse(unsigned int wait_us) const {
    // A return value of 0 means the communication failed.
    unsigned int begin = 0;
    unsigned int end = 0;
    unsigned int duration = 0;
    unsigned timeout = micros() + wait_us;

    // Wait for REST state
    while (digitalRead(_pin) == _active_state) {
    }

    // Wait for ACTIVE state marking the pulse start
    while (digitalRead(_pin) != _active_state && begin < timeout) {
      begin = micros();
    }

    // Wait for REST state marking the pulse end
    while (digitalRead(_pin) == _active_state) {
      end = micros();
    }

    if (end != 0) {
      duration = end - begin;
    }
    return duration;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 2;
    unsigned int index = 0;
    unsigned int value = 0;

    // static unsigned int min = 1000;
    // static unsigned int max = 0;

    // DEBUG_INFO("Pulses: ");
    for (int pulse : pulses) {
      if (pulse > threshold) {
        bitSet(value, index);
      }
      index++;
      // Serial.print(pulse);
      // Serial.print(", ");
      // if (pulse > 0) {
      //   if (pulse < min) {
      //     min = pulse;
      //   }
      //   if (pulse > max) {
      //     max = pulse;
      //   }
      // }
    }
    // DEBUG_INFO(" (%d, %d)", min, max);

    return value;
  }

public:
  static void Setup(const unsigned int pin, const unsigned int msgLength,
                    const unsigned int frequency) {
    BitBang &instance = GetInstance();

    instance._pin = pin;
    instance._active_state = LOW;
    instance.closeChannel();

    instance._tick = 1E6 / frequency / 4;
    instance.msgLength = msgLength;
    instance.connection = false;

    delay(50); // Delay necessary for both boards to catch up
    DEBUG_INFO("Waiting for channed to close.");
    while (!instance.isChannelClosed()) {
    }
    DEBUG_INFO("Channel closed.");

    DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
               instance._active_state);
    DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d", instance._tick,
               int(1E6 / (instance._tick * 4)));
  }

  static bool Send(const unsigned int &value) {
    static const BitBang &instance = GetInstance();
    DEBUG_INFO("Send value: %d", value);

    // Send GO
    // unsigned int begin = micros();
    DEBUG_INFO("Waiting on channel to open");
    while (!instance.isChannelOpened()) {
    }
    DEBUG_INFO("Channel has been opened");
    noInterrupts();
    instance.sendBit(0);

    // Send bits, LSB first.
    for (int i = 0; i < instance.msgLength; ++i) {
      instance.sendBit(bitRead(value, i));
    }
    interrupts();
    while (!instance.isChannelClosed()) {
      DEBUG_INFO("Waiting for channel to close");
    }
    DEBUG_INFO("Channel has been closed");
    // DEBUG_INFO("Time: %d",(micros() - begin) / 1000.0);

    DEBUG_INFO("Sent bits: ");
    Serial.println(value, BIN);
    return true;
  }

  static bool Receive() {
    static BitBang &instance = GetInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(instance.msgLength);

    // Wait for GO
    DEBUG_INFO("Opening channel");
    instance.openChannel();
    DEBUG_INFO("Channel opened");
    noInterrupts();
    // unsigned int begin = micros();
    if (instance.receivePulse(5000) == 0) {
      // TODO: Handle connection failures
      instance.closeChannel();
      interrupts();
      DEBUG_INFO("Connection failed");
      return false;
    }

    // Read bits
    // unsigned int end = micros();
    for (int i = instance.msgLength; i > 0; --i) {
      pulses.push_back(instance.receivePulse(instance._tick * 4));
    }
    instance.closeChannel();
    interrupts();
    // DEBUG_INFO("Wait Time (ms): %d", (end - begin) / 1000.0);
    // DEBUG_INFO("Transmission Time (ms): %d", (micros() - begin) / 1000.0);

    // Decode pulse
    instance.values.push_back(instance.decodePulses(pulses));
    DEBUG_INFO("Received value: %d", instance.values.back());

    return true;
  }

  static unsigned int GetValue() {
    static BitBang &instance = GetInstance();
    unsigned int value = instance.values.front();
    instance.values.pop_front();
    return value;
  }
};

} // namespace txrx
} // namespace hardware
#endif