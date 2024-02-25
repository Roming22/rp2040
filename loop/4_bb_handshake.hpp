// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  DEBUG_INFO("## [Loop %d] %s", ++loopIndex,
             isLeft ? "Controller" : "Extension");
  setLed(0, (loopIndex - 1) % 6);
  bool handshake = false;
  while (!handshake) {
    setLed(2, 4);
    DEBUG_INFO("Handshake in progress");
    handshake = BitBang::Handshake();
    if (handshake) {
      DEBUG_INFO("Handshake OK");
      setLed(1, 2);
    } else {
      DEBUG_INFO("Handshake FAILED");
      setLed(1, 0);
    }
  }
  setLed(2, 1);
  if (loopIndex % (3001) == 0 && isLeft) {
    delay(1200);
  }
}
