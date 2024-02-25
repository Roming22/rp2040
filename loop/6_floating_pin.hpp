void loop() {
  if (isLeft) {
    pinMode(DATA_PIN, INPUT_PULLUP);
    setLed(1, 2 * digitalRead(DATA_PIN));
    DEBUG_INFO("Read: %d", digitalRead(DATA_PIN));
    delay(3000);
  } else {
    // digitalWrite(DATA_PIN, LOW);
    // pinMode(DATA_PIN, INPUT);
    pinMode(DATA_PIN, INPUT_PULLUP);
  }
}
