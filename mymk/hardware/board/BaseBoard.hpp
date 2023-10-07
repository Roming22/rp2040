#ifndef MYMK_HARDWARE_BOARD_BASEBOARD
#define MYMK_HARDWARE_BOARD_BASEBOARD

#include "../BitBang.hpp"
#include "../Pixels.hpp"
#include "../switch/Key.hpp"
#include "../switch/KeyMatrix.hpp"

#include <string>
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
  std::vector<int> switch_events;

public:
  BaseBoard(const unsigned int &i_msg_len,
            const std::vector<unsigned int> &i_col_pins,
            const std::vector<unsigned int> &i_row_pins)
      : msg_len(i_msg_len), is_connected(true) {
    key = new KeyMatrix(i_col_pins, i_row_pins);
  }

  void load_switch_events() {
    Serial.println("BaseBoard::load_switch_events");
    switch_events.clear();
    key->poll_events(switch_events);
  }

  void send_switch_events() {
    Serial.println("BaseBoard::send_switch_events");
    if (!is_connected) {
      return;
    }

    int event;
    for (unsigned int i = 0; i < switch_events.size(); ++i) {
      // Serial.println("POST Value");
      event = switch_events[i];
      Serial.print("Send event: ");
      Serial.println(event);
      BitBang::Send(event, msg_len);
    }
    // Serial.println("POST Value");
    BitBang::Send(0, msg_len);
  }

  void receive_switch_events() {
    Serial.println("BaseBoard::receive_switch_events");
    int event = 1;
    while (event != 0 && is_connected) {
      // Serial.println("GET Value");
      event = BitBang::Receive(msg_len);
      if (event > 0) {
        Serial.println("Received Press event");
        switch_events.push_back(event + key->size);
      } else if (event < 0) {
        Serial.println("Received Release event");
        switch_events.push_back(event - key->size);
      }
    }
  }

  void process_switch_events() {
    Serial.println("BaseBoard::process_switch_events");
    int event;
    for (int i = 0; i < switch_events.size(); ++i) {
      event = switch_events[i];
      Serial.print("Switch Event: ");
      Serial.println(event);
      blinkLeds(100);
    }
  }

  virtual void loop() {
    Serial.println("[ERROR] BaseBoard::loop is not implemented");
    delay(3600000);
  }

  static void Loop() {
    Serial.println("BaseBoard::Loop");
    instance->loop();
  };
};
BaseBoard *BaseBoard::instance = nullptr;
#endif