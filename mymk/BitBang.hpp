#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#include <cmath>
#include <functional>

class BitBang {
  unsigned int _pin;
  bool _active_state;
  unsigned int _tick;

  // Singleton
  BitBang() {}

  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  static BitBang &getInstance() {
    static BitBang instance;
    return instance;
  }

  inline void resetPin() const {
    pinMode(_pin, OUTPUT);
    gpio_put(_pin, !_active_state);
  }

  inline void inputPin() const { pinMode(_pin, INPUT_PULLUP); }

  inline void wait_until(unsigned long time) const {
    while (micros() < time) {
    }
  }

  inline void sendBit(const bool &bit) const {
    unsigned long time;
    time = micros() + (_tick * (bit ? 3 : 1));
    gpio_put(_pin, _active_state);
    wait_until(time);
    time = micros() + (_tick * (bit ? 1 : 3));
    gpio_put(_pin, !_active_state);
    wait_until(time);
  }

  inline unsigned int receivePulse(unsigned int wait_us) const {
    // A return value of 0 means the communication failed.
    unsigned int begin = 0;
    unsigned int end = 0;
    unsigned int duration = 0;
    unsigned timeout = micros() + wait_us;

    // Wait for REST state
    while (gpio_get(_pin) == _active_state) {
    }

    // Wait for ACTIVE state marking the pulse start
    while (gpio_get(_pin) != _active_state && begin < timeout) {
      begin = micros();
    }

    // Wait for REST state marking the pulse end
    while (gpio_get(_pin) == _active_state) {
      end = micros();
    }

    if (end != 0) {
      duration = end - begin;
    }
    return duration;
  }

  inline void sendSyncSignal() const {
    unsigned int begin = micros();
    gpio_put(_pin, _active_state);
    while (micros() - begin < _tick * 4) {
    }
    begin = micros();
    gpio_put(_pin, !_active_state);
    while (micros() - begin < _tick) {
    }
    Serial.println("Sync signal sent");
  }

  inline bool receiveSyncSignal(unsigned int wait_us = 1E6) const {
    int signal = receivePulse(wait_us);
    Serial.print("Sync signal: ");
    Serial.println(signal);
    return signal > _tick * 3.5;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 2;
    unsigned int index = 0;
    unsigned int value = 0;

    // static unsigned int min = 1000;
    // static unsigned int max = 0;

    // Serial.print("Pulses: ");
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
    // Serial.print(" (");
    // Serial.print(min);
    // Serial.print(",");
    // Serial.print(max);
    // Serial.println(")");

    return value;
  }

public:
  static void initialize(const unsigned int pin, const unsigned int frequency) {
    BitBang &instance = getInstance();
    instance._pin = pin;
    instance._active_state = LOW;
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

  static inline bool sendSync(unsigned int wait_us = 1E6) {
    static const BitBang &instance = getInstance();
    if (!instance.receiveSyncSignal(wait_us)) {
      return false;
    }
    instance.sendSyncSignal();
    return true;
  }

  static inline bool receiveSync(unsigned int wait_us = 1E6) {
    static const BitBang &instance = getInstance();
    unsigned int timeout = micros() + wait_us;
    while (micros() < timeout) {
      instance.sendSyncSignal();
      if (instance.receiveSyncSignal(instance._tick * 16)) {
        return true;
      }
    }
    return false;
  }

  static void send(const unsigned int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    // Serial.print("Send value: ");
    // Serial.println(value);

    // Send GO
    // unsigned int begin = micros();
    noInterrupts();
    instance.sendBit(0);

    // Send bits, LSB first.
    for (int i = 0; i < length; ++i) {
      instance.sendBit(bitRead(value, i));
    }
    interrupts();
    // Serial.print("Time: ");
    // Serial.println((micros() - begin) / 1000.0);

    // Serial.print("Sent bits: ");
    // Serial.print(value, BIN);
    // Serial.println();
  }

  static unsigned int receive(const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(length);

    instance.inputPin();

    // Wait for GO
    noInterrupts();
    unsigned int begin = micros();
    while (instance.receivePulse(1E6) == 0) {
    }

    // Read bits
    unsigned int end = micros();
    for (int i = length; i > 0; --i) {
      pulses.push_back(instance.receivePulse(instance._tick * 4));
    }
    interrupts();
    Serial.print("Wait Time (ms): ");
    Serial.println((end - begin) / 1000.0);
    Serial.print("Transmission Time (ms): ");
    Serial.println((micros() - begin) / 1000.0);
    instance.resetPin();

    // Decode pulse
    int value = instance.decodePulses(pulses);

    // if (value > 0) {
    //   Serial.print("Received bits: ");
    //   Serial.println(value, BIN);
    //   Serial.print("Received value: ");
    //   Serial.println(value);
    // }

    return value;
  }
};

void set_bitbang() { BitBang::initialize(DATA_PIN, DATA_FREQ); }

#endif
