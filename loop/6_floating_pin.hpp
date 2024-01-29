// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop() {
  if (isLeft) {
    // Close channel
    DEBUG_INFO("Close channel");
    pinMode(DATA_PIN, OUTPUT);
    digitalWrite(DATA_PIN, HIGH);
    setLed(1, 0);
    // delay(300 * random(6));
    delay(2000);

    // Open channel
    DEBUG_INFO("Open channel");
    digitalWrite(DATA_PIN, LOW);
    // pinMode(DATA_PIN, INPUT);
    setLed(1, 4);
    // delay(300 * random(6));
    delay(2000);

  } else {
    // digitalWrite(DATA_PIN, LOW);
    // pinMode(DATA_PIN, INPUT);
    digitalWrite(DATA_PIN, HIGH);
    pinMode(DATA_PIN, INPUT_PULLUP);

    bool state = !digitalRead(DATA_PIN);
    while (true) {
      if (digitalRead(DATA_PIN) != state) {
        state = !state;
        DEBUG_INFO("Channel is %s %d", state ? "opened" : "closed", 4 * state);
        setLed(1, 4 * !state);
      }
    }
  }
}
