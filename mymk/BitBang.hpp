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
  static void initialize(const unsigned int pin, const unsigned int frequency) {
    BitBang &instance = getInstance();
    instance._pin = pin;
    instance._active_state = LOW;
    instance.closeChannel();

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

  static void send(const unsigned int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    // DEBUG_INFO("Send value: %d",value);

    // Send GO
    // unsigned int begin = micros();
    noInterrupts();
    while (!instance.isChannelOpened()) {
    }
    instance.sendBit(0);

    // Send bits, LSB first.
    for (int i = 0; i < length; ++i) {
      instance.sendBit(bitRead(value, i));
    }
    interrupts();
    // DEBUG_INFO("Time: %d",(micros() - begin) / 1000.0);

    // DEBUG_INFO("Sent bits: ");
    // Serial.println(value, BIN);
  }

  static unsigned int receive(const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(length);

    // Wait for GO
    noInterrupts();
    instance.openChannel();
    // unsigned int begin = micros();
    if (instance.receivePulse(5000) == 0) {
      return 1000000;
    }

    // Read bits
    // unsigned int end = micros();
    for (int i = length; i > 0; --i) {
      pulses.push_back(instance.receivePulse(instance._tick * 4));
    }
    instance.closeChannel();
    interrupts();
    // DEBUG_INFO("Wait Time (ms): %d", (end - begin) / 1000.0);
    // DEBUG_INFO("Transmission Time (ms): %d", (micros() - begin) / 1000.0);

    // Decode pulse
    int value = instance.decodePulses(pulses);

    // if (value > 0) {
    //   DEBUG_INFO("Received bits: ");
    //   Serial.println(value, BIN);
    //   DEBUG_INFO("Received value: %d",value);
    // }

    return value;
  }
};

void set_bitbang() { BitBang::initialize(DATA_PIN, DATA_FREQ); }

#endif