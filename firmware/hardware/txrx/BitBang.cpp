#include "BitBang.h"

namespace hardware {
namespace txrx {

void BitBang::Setup(const unsigned int i_pin,
                    const unsigned int i_message_length,
                    const unsigned int i_frequency,
                    const unsigned int i_input_mode) {
  BitBang &instance = getInstance();
  instance.pin = i_pin;
  instance.input_mode = i_input_mode;
  instance.active_state = (instance.input_mode == INPUT);
  instance._resetPin();

  instance.msg_len = i_message_length;
  instance.pulses.reserve(i_message_length);

  instance.tick = 1E6 / i_frequency / 4;

  // DEBUG_INFO("LOW: %d    HIGH: %d    ACTIVE: %d", LOW, HIGH,
  //            instance.active_state);
  DEBUG_INFO("BitBang tick (usec): %d    Frequency (Hz): %d", instance.tick,
             int(1E6 / (instance.tick * 4)));
}

unsigned int BitBang::_decodePulses() {
  static unsigned int threshold = tick * 3 / 2;
  unsigned int pulse;
  unsigned int value = 0;

  static unsigned int min = 1000;
  static unsigned int max = 0;

  // DEBUG_INFO("Pulses: ");
  for (int i = pulses.size() - 1; i >= 0; --i) {
    pulse = pulses.back();
    if (pulse > threshold) {
      bitSet(value, i);
    }
    pulses.pop_back();
    // Serial.print(pulse);
    // Serial.print(", ");
    if (pulse > 0) {
      if (pulse < min) {
        min = pulse;
      }
      if (pulse > max) {
        max = pulse;
      }
    }
  }
  // DEBUG_INFO("(min: %d    max: %d)", min, max);

  return value;
}

void BitBang::SendData(const unsigned int &value) {
  static const BitBang &instance = getInstance();
  // Serial.print("Send value: ");
  // Serial.println(value);

  // Send GO
  noInterrupts();
  instance._sendBit(!instance.active_state);

  // Send bits, LSB first.
  instance._sendData(value, instance.msg_len);
  interrupts();

  // Serial.print("Sent bits: ");
  // Serial.println(value, BIN);
}

unsigned int BitBang::ReceiveData() {
  static BitBang &instance = getInstance();
  unsigned int value;

  //- noInterrupts();
  instance._inputPin();

  // Wait for GO
  while (instance._receivePulse() == 0) {
  }

  // Read bits
  instance._receiveData(instance.msg_len);
  //- interrupts();

  // Decode pulse
  value = instance._decodePulses();

  // if (value > 0) {
  //   Serial.print("Received bits: ");
  //   Serial.println(value, BIN);
  //   DEBUG_INFO("Received value: %d", value);
  // }

  return value;
}

void BitBang::Send(const int &value) {
  static BitBang &instance = getInstance();
  DEBUG_DEBUG("Send value: %d", value);

  noInterrupts();
  instance._sendSyncSignal();
  busy_wait_us_32(instance.tick * 4);
  instance._sendData(value, instance.msg_len);
  interrupts();

  // Serial.print("Sent bits: ");
  // Serial.println(value, BIN);
}

unsigned int BitBang::Receive() {
  static BitBang &instance = getInstance();
  instance.pulses.reserve(instance.msg_len);
  static unsigned int value;

  noInterrupts();
  instance._receiveSyncSignal();
  instance._receiveData(instance.msg_len);
  interrupts();

  // Decode pulse
  value = instance._decodePulses();

  // Serial.print("Received bits: ");
  // Serial.println(value, BIN);
  DEBUG_DEBUG("Received value: %d", value);

  return value;
}
} // namespace txrx
} // namespace hardware