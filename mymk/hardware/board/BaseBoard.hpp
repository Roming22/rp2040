#ifndef MYMK_HARDWARE_BOARD_BASEBOARD
#define MYMK_HARDWARE_BOARD_BASEBOARD

#include "../BitBang.hpp"
#include "../Pixels.hpp"
#include "../switch/Key.hpp"
#include "../switch/KeyMatrix.hpp"
#include <vector>

void blinkLeds(unsigned int duration) {
  static int color = 0;
  static Pixels &pixels = *Pixels::get();

  int red = 255 * (color == 0 || color == 1 || color == 5);
  int green = 255 * (color == 1 || color == 2 || color == 3);
  int blue = 255 * (color == 3 || color == 4 || color == 5);

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
  delay(duration);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
  color = ++color % 6;
}

void error(unsigned int duration) {
  static Pixels &pixels = *Pixels::get();

  pixels.fill(pixels.Color(255, 255, 255));
  pixels.show();
  delay(duration);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
}

class BaseBoard {
protected:
  static BaseBoard *instance;
  Key *key;
  unsigned int msg_len;
  bool is_connected;
  bool is_motherboard;

public:
  BaseBoard(const unsigned int &i_msg_len,
            const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins)
      : msg_len(i_msg_len), is_connected(true) {
    key = new KeyMatrix(i_col_pins, i_row_pins);
  }

  void send_switch_events(const std::vector<int> &events) {
    if (!is_connected) {
      return;
    }

    for (unsigned int i = 0; i < events.size(); ++i) {
      // Serial.println("POST Value");
      BitBang::Send(events[i], msg_len);
    }
    // Serial.println("POST Value");
    BitBang::Send(0, msg_len);
  }

  virtual void receive_switch_events(std::vector<int> &events) {
    int event = 1;
    while (event != 0 && is_connected) {
      // Serial.println("GET Value");
      event = BitBang::Receive(msg_len);
      if (event > 0) {
        events.push_back(event + key->size);
      } else if (event < 0) {
        events.push_back(event - key->size);
      }
    }
  }

  void process_switch_events(const std::vector<int> &events) {
    // Serial.println("Process switch events");
    int event;
    // Process events
    for (int i = 0; i < events.size(); ++i) {
      event = events[i];
      Serial.print("Switch Event: ");
      Serial.println(event);
      blinkLeds(100);
    }
  }

  virtual void loop() {
    Serial.println("[ERROR] Unimplemented");
    delay(3600000);
  }

  static void Loop() { instance->loop(); };
};
BaseBoard *BaseBoard::instance = nullptr;
#endif