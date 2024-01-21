#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#include "../../utils/Debug.hpp"

#include <cmath>
#include <list>

namespace hardware {
namespace txrx {

class BitBang {
  unsigned int pin;
  unsigned int msg_length;
  bool active_state;
  unsigned int tick;
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
    noInterrupts();
    digitalWrite(pin, !active_state);
    pinMode(pin, INPUT_PULLUP);
  }

  inline void closeChannel() const {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, active_state);
    interrupts();
  }

  inline bool isChannelOpened() const {
    bool quiet = false;
    openChannel();
    while (!quiet) {
      while (!quiet) {
        quiet = digitalRead(pin) != active_state;
      }
      unsigned long timeout = micros() + tick * 8;
      while (quiet && micros() < timeout) {
        quiet = digitalRead(pin) != active_state;
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
        closed = digitalRead(pin) == active_state;
      }
      unsigned long timeout = micros() + tick * 8;
      while (closed && micros() < timeout) {
        closed = digitalRead(pin) == active_state;
      }
    }
    closeChannel();
    return closed;
  }

  inline void sendBit(const bool &bit) const {
    unsigned long time;
    time = micros() + (tick * (bit ? 3 : 1));
    digitalWrite(pin, active_state);
    wait_until(time);
    time = micros() + (tick * (bit ? 1 : 3));
    digitalWrite(pin, !active_state);
    wait_until(time);
  }

  inline unsigned int receivePulse(unsigned int wait_us) const {
    // A return value of 0 means the communication failed.
    unsigned long int begin = 0;
    unsigned long int end = 0;
    unsigned int duration = 0;
    unsigned timeout = micros() + wait_us;

    // Wait for REST state
    while (digitalRead(pin) == active_state) {
      if (micros() > timeout) {
        // DEBUG_INFO("REST state not detected");
        return 0;
      }
    }

    // Wait for ACTIVE state marking the pulse start
    begin = micros();
    while (digitalRead(pin) != active_state) {
      begin = micros();
      if (begin > timeout) {
        // DEBUG_INFO("REST state for too long");
        return 0;
      }
    }

    // Wait for REST state marking the pulse end
    timeout = micros() + tick * 10;
    while (digitalRead(pin) == active_state) {
      end = micros();
      if (end > timeout) {
        // DEBUG_INFO("ACTIVE state for too long");
        return 0;
      }
    }

    if (end > 0) {
      // Serial.println("Full pulse");
      duration = end - begin;
      if (duration > 100) {
        DEBUG_INFO("Bad pulse: %d  (%d-%d)", duration, end, begin);
      }
    }
    if (duration > 100) {
      DEBUG_INFO("Bad NO pulse: %d", duration);
    }
    return duration;
  }

  int decodePulses(std::vector<unsigned int> &pulses) const {
    static unsigned int threshold = tick * 2;
    unsigned int index = 0;
    unsigned int value = 0;

    // static unsigned int min = 1000;
    // static unsigned int max = 0;

    for (int pulse : pulses) {
      if (pulse > threshold) {
        bitSet(value, index);
      }
      index++;
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
    if (value > 0) {
      DEBUG_INFO("Pulses: ");
      for (int pulse : pulses) {
        Serial.print(pulse);
        Serial.print(", ");
      }
      Serial.println(" -- End");
      Serial.print("Pulse: ");
      Serial.println(value, BIN);
    }

    return value;
  }

public:
  static void Setup(const unsigned int pin, const unsigned int msgLength,
                    const unsigned int frequency) {
    BitBang &instance = GetInstance();

    instance.pin = pin;
    instance.active_state = LOW;
    instance.closeChannel();

    instance.tick = 1E6 / (frequency * 4);
    instance.msg_length = msgLength;

    delay(200); // Delay necessary for both boards to catch up
    // DEBUG_INFO("Waiting for channed to close.");
    while (!instance.isChannelClosed()) {
    }
    // DEBUG_INFO("Channel closed.");

    DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
               instance.active_state);
    DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d", instance.tick,
               int(1E6 / (instance.tick * 4)));
  }

  static bool Send(const unsigned int &value) {
    static const BitBang &instance = GetInstance();
    // DEBUG_INFO("Send value: %d", value);

    // Send GO
    // unsigned int begin = micros();
    // DEBUG_INFO("Waiting on channel to open");
    noInterrupts();
    while (!instance.isChannelOpened()) {
    }
    // DEBUG_INFO("Channel has been opened");
    instance.sendBit(1);
    // instance.wait_until(micros() + 20);

    // Send bits, LSB first.
    for (int i = 0; i < instance.msg_length; ++i) {
      instance.sendBit(bitRead(value, i));
    }
    interrupts();
    while (!instance.isChannelClosed()) {
      // DEBUG_INFO("Waiting for channel to close");
    }
    // DEBUG_INFO("Channel has been closed");
    // DEBUG_INFO("Time: %d",(micros() - begin) / 1000.0);

    // DEBUG_INFO("Sent bits: ");
    // Serial.println(value, BIN);
    return true;
  }

  static bool Receive() {
    static BitBang &instance = GetInstance();
    std::vector<unsigned int> pulses;
    pulses.reserve(instance.msg_length);

    // Wait for GO
    // DEBUG_INFO("Opening channel");
    instance.openChannel();
    // DEBUG_INFO("Channel opened");
    // unsigned int begin = micros();
    if (instance.receivePulse(5000) == 0) {
      // TODO: Handle connection failures
      instance.closeChannel();
      // DEBUG_INFO("Connection failed");
      return false;
    }

    // Read bits
    // unsigned int end = micros();
    for (int i = instance.msg_length; i > 0; --i) {
      pulses.push_back(instance.receivePulse(100));
    }
    instance.closeChannel();
    // DEBUG_INFO("Wait Time (ms): %d", (end - begin) / 1000.0);
    // DEBUG_INFO("Transmission Time (ms): %d", (micros() - begin) / 1000.0);

    // Decode pulse
    instance.values.push_back(instance.decodePulses(pulses));
    // DEBUG_INFO("Received value: %d", instance.values.back());

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