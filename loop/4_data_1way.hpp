// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  DEBUG_INFO("## Loop %d [%s] %s Value", ++loopIndex,
             isLeft ? "Controller" : "Extension", isLeft ? "GET" : "POST");

  if (isLeft) {
    // delay(1000); This delay does not break the communication
    setLed(1, 0);
    msg = BitBang::receive(msgLen);
  } else {
    setLed(1, 0);
    BitBang::send(loopIndex, msgLen);
    msg = loopIndex;
  }
  setLed(1, 2);

  setLed(0, (loopIndex - 1) % 6);

  if (loopIndex % 100 == 0 && inError) {
    DEBUG_INFO("Error count: %d", inError);
    delay(2000);
  }

  if (msg != loopIndex) {
    ++inError;
    if (msg > 10) {
      DEBUG_INFO("!!! Bad value #%d: %d", ++inError, msg);
    } else {
      inError = 0;
    }
    loopIndex = msg;
  }
}
