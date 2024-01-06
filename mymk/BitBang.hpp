#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#include <cmath>
#include <functional>
#include <vector>

class BitBang {
  unsigned int _pin;
  unsigned int _input_mode;
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

  inline void resetPin() const {
    pinMode(_pin, OUTPUT);
    gpio_put(_pin, !_active_state);
  }

  inline void inputPin() const { pinMode(_pin, _input_mode); }

  inline void sendBit(const bool &bit) const {
    unsigned long time = micros() + (_tick * (bit ? 3 : 1));
    gpio_put(_pin, _active_state);
    wait_until(time);
    time = micros() + (_tick * (bit ? 1 : 3));
    gpio_put(_pin, !_active_state);
    wait_until(time);
  }

  inline bool receivePulses(std::vector<unsigned int> &pulses,
                            unsigned int length, unsigned int timeout) const {
    inputPin();
    unsigned int pulse;
    for (; length > 0; --length) {
      pulse = receivePulse(timeout);
      pulses.push_back(pulse);
      if (pulse == 0) {
        // Serial.print("FAILED Received pulse after #: ");
        // Serial.println(pulses.size());
        pulses.clear();
        break;
      }
    }
    resetPin();
    // Serial.print("Received pulse #: ");
    // Serial.println(pulses.size());
    return !pulses.empty();
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
    // Serial.print("start");

    // Wait for ACTIVE state marking the pulse start
    while (gpio_get(_pin) != _active_state && begin < timeout) {
      begin = micros();
      // Serial.print(":");
    }

    // Wait for REST state marking the pulse end
    while (gpio_get(_pin) == _active_state) {
      end = micros();
      // Serial.print(".");
    }

    if (end != 0) {
      duration = end - begin;
    }
    // Serial.println("end");
    return duration;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = _tick * 3 / 2;
    unsigned int pulse;
    unsigned int index = 0;
    unsigned int value = 0;

    static unsigned int min = 1000;
    static unsigned int max = 0;

    // Serial.print("Pulses [");
    // Serial.print(pulses.size());
    // Serial.print("]: ");
    for (int pulse : pulses) {
      // Serial.print(pulse);
      // Serial.print(", ");
      if (pulse > threshold) {
        bitSet(value, index);
      }
      index++;
      if (pulse > 0) {
        if (pulse < min) {
          min = pulse;
        }
        if (pulse > max) {
          max = pulse;
        }
      }
    }
    // Serial.print("Pulses value: ");
    // Serial.println(value);
    // Serial.print(" (");
    // Serial.print(min);
    // Serial.print(",");
    // Serial.print(max);
    // Serial.println(")");
    pulses.clear();
    return value;
  }

public:
  static void initialize(const unsigned int pin, const unsigned int frequency,
                         const unsigned input_mode = INPUT) {
    BitBang &instance = getInstance();
    instance._pin = pin;
    instance._input_mode = input_mode;
    instance._active_state = (instance._input_mode == LOW);
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

  void channelOpened() const {
    bool quiet = false;
    inputPin();
    while (!quiet) {
      while (!quiet) {
        quiet = gpio_get(_pin) != _active_state;
      }
      unsigned long timeout = micros() + _tick * 4;
      while (quiet && micros() < timeout) {
        quiet = gpio_get(_pin) != _active_state;
      }
    }
    resetPin();
    return;
  }

  static void send(const unsigned int &value, const unsigned int &length) {
    static const BitBang &instance = getInstance();
    std::vector<unsigned int> pulses;
    bool synced = false;
    // Serial.print("Send value: ");
    // Serial.println(value);

    // unsigned int begin = micros();
    noInterrupts();
    instance.channelOpened();
    // Serial.println("Channel open (Send)");

    // Wait for GO
    while (!synced) {
      synced = instance.receivePulses(pulses, 1, instance._tick * 16) &&
               instance.decodePulses(pulses) == 1;
    }
    // Send ACK
    unsigned long time = micros() + instance._tick * 2;
    instance.wait_until(time);
    instance.sendBit(1);
    Serial.println("Synced");

    // // Send bits, LSB first.
    // for (int i = 0; i < length; ++i) {
    //   instance.sendBit(bitRead(value, i));
    // }
    interrupts();
    // // Long delay to signify end of transmission
    // time = micros() + instance._tick * 4;
    // instance.wait_until(time);

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
    bool synced = false;

    unsigned int begin = micros();
    noInterrupts();
    instance.channelOpened();
    // Serial.println("Channel open (Receive)");
    while (!synced) {
      // Send GO
      instance.sendBit(1);
      // Check for ACK
      synced = instance.receivePulses(pulses, 1, instance._tick * 16) &&
               instance.decodePulses(pulses) == 1;
    }
    Serial.println("Synced");

    // // Read bits
    // unsigned int end = micros();
    // instance.receivePulses(pulses, length, instance._tick * 400);

    interrupts();
    // Serial.print("Wait Time (ms): ");
    // Serial.println((end - begin) / 1000.0);
    // Serial.print("Transmission Time (ms): ");
    // Serial.println((micros() - begin) / 1000.0);

    // // Decode pulse
    // int value = instance.decodePulses(pulses);

    // if (value > 0) {
    //   Serial.print("Received bits: ");
    //   Serial.println(value, BIN);
    //   Serial.print("Received value: ");
    //   Serial.println(value);
    // }

    return 0;
    // return value;
  }
};

void set_bitbang() { BitBang::initialize(DATA_PIN, DATA_FREQ); }

#endif
