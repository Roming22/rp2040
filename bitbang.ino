#include <string>

#define DATA_PIN 1
#define DATA_FREQ 125000
#define PIXELS_PIN 0
#define PIXELS_COUNT 4
#define VUSB_PIN 19

#define LOOP 6

bool isLeft = 0;
bool isUsbConnected = true;

#include "mymk/mymk.hpp"

#if LOOP == 1
#include "loop/1_relay.hpp"
#endif
#if LOOP == 2
#include "loop/2_echo.hpp"
#endif
#if LOOP == 3
#include "loop/3_sync.hpp"
#endif
#if LOOP == 4
#include "loop/4_data_1way.hpp"
#endif
#if LOOP == 5
#include "loop/5_data_2way.hpp"
#endif
#if LOOP == 6
#include "loop/6_floating_pin.hpp"
#endif

void setup() {
  Serial.begin(9600);
  delay(1000);

  Debug.timestampOn();
  // Debug.setDebugLevel(DBG_NONE);
  // Debug.setDebugLevel(DBG_ERROR);
  // Debug.setDebugLevel(DBG_WARNING);
  Debug.setDebugLevel(DBG_INFO);
  // Debug.setDebugLevel(DBG_DEBUG);
  // Debug.setDebugLevel(DBG_VERBOSE);

  for (int i = 10; i > 0; --i) {
    Serial.println();
  }
  Serial.println("[BITBANG]");
  Serial.println("#############################################################"
                 "###################");
  Serial.println("# Rebooting");

  set_board();
  set_pixels(isLeft);
  BitBang::initialize(DATA_PIN, DATA_FREQ, isUsbConnected);

  Serial.println("");
  Serial.println("# Looping");
}
