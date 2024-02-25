#ifndef MYMK_BITBANG
#define MYMK_BITBANG

#include <cmath>
#include <functional>
#include <ratio>
#include <vector>

class BitBang {
  unsigned int pin;
  bool pinWriteMode;
  bool active_state;
  unsigned int tick_us;
  unsigned int ticks_per_frame;
  bool is_host;

  // Singleton
  BitBang() {}

  // Singleton
  BitBang(const BitBang &obj) = delete;
  BitBang &operator=(const BitBang &obj) = delete;

  static BitBang &getInstance() {
    static BitBang instance;
    return instance;
  }

  inline void wait_for(unsigned long tick_count) const {
    unsigned long time = micros() + tick_us * tick_count;
    while (micros() < time) {
    }
  }

  inline void reset_pin() {
    if (pinWriteMode) {
      pinMode(pin, INPUT_PULLUP);
      pinWriteMode = false;
    }
  }

  inline void set_pin(bool state, unsigned long tick_count) {
    if (!pinWriteMode) {
      pinMode(pin, OUTPUT);
      pinWriteMode = true;
    }
    unsigned long time = micros() + tick_us * tick_count;
    digitalWrite(pin, state ? active_state : !active_state);
    while (micros() < time) {
    }
    digitalWrite(pin, !active_state);
  }

  inline bool get_pin() {
    int low_count = 0;
    int high_count = 0;
    int samples = 3;

    if (pinWriteMode) {
      pinMode(pin, INPUT_PULLUP);
      pinWriteMode = false;
    }
    while (low_count < samples && high_count < samples) {
      if (digitalRead(pin)) {
        ++high_count;
        low_count = 0;
      } else {
        ++low_count;
        high_count = 0;
      }
    }
    // DEBUG_INFO("Signal: %d (low: %d, high: %d)",
    //            (high_count > low_count) == active_state, low_count,
    //            high_count);
    return (high_count > low_count) == active_state;
  }

  inline bool channel_is_opened() {
    /*
    Makes sure that the channel is opened by checking that the
    pin is an inactive state for longer than a frame.
    */
    bool opened = true;
    unsigned long timeout = micros() + tick_us * (ticks_per_frame + 1);
    while (micros() < timeout && opened) {
      opened = !get_pin();
    }
    return opened;
  }

  bool handshake(unsigned int timeout_ms) {
    bool success = false;
    unsigned long timeout = micros() + timeout_ms * 1E3;
    unsigned long ack_duration = 1;
    unsigned long ack_timeout_duration =
        ack_duration * 4 * tick_us * ticks_per_frame;
    unsigned long ack_timeout;
    if (is_host) {
      //
      // HOST
      //
      while (!success && micros() < timeout) {
        while (!channel_is_opened() && micros() < timeout) {
        }

        // DEBUG_INFO("Wait for SIG");
        while (!success && micros() < timeout) {
          success = get_pin();
        }
        while (success && get_pin()) {
          // Wait line to go back to inactive.
        }

        if (success) {
          // DEBUG_INFO("Send ACK");
          unsigned long time = micros() + 200;
          while (micros() < time) {
          }
          set_pin(true, ack_duration);
          reset_pin();
        }
      }
    } else {
      //
      // DEVICE
      //
      while (!success && micros() < timeout) {
        while (!channel_is_opened() && micros() < timeout) {
        }

        // DEBUG_INFO("Send SIG");
        ack_timeout = micros() + ack_timeout_duration;
        set_pin(true, ack_duration);
        reset_pin();

        // DEBUG_INFO("Wait for ACK");
        while (!success && micros() < ack_timeout) {
          success = get_pin();
        }
      }
    }
    return success;
  }

public:
  static void initialize(const unsigned int pin, const unsigned int frequency,
                         const bool isUsbConnected) {
    BitBang &instance = getInstance();
    instance.pin = pin;
    instance.ticks_per_frame = 4;
    instance.active_state = LOW;
    instance.reset_pin();
    instance.is_host = isLeft;
    instance.tick_us = 1E6 / frequency / instance.ticks_per_frame;
    if (instance.tick_us < 2) {
      DEBUG_INFO("Tick is bad");
      instance.tick_us = 2;
    }

    DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
               instance.active_state);
    DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d",
               instance.tick_us,
               int(1E6 / (instance.tick_us * instance.ticks_per_frame)));

    unsigned long timeout = micros() + 500 * 1E3;
    while (!instance.channel_is_opened() && micros() < timeout) {
    }
  }

  static bool Handshake(unsigned int timeout_ms = 5) {
    BitBang &instance = getInstance();
    bool success = false;

    noInterrupts();
    success = instance.handshake(timeout_ms);
    interrupts();

    // Timeout
    if (success) {
      // while (!instance.channel_is_opened() && micros() < timeout) {
      // }
    } else {
      DEBUG_INFO("Timeout: No %s detected",
                 instance.is_host ? "device" : "host");
    }
    return success;
  }
};
#endif