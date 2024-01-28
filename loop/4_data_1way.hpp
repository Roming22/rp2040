// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  DEBUG_INFO("## [Loop %d] %s %s Value", ++loopIndex,
             isLeft ? "Controller" : "Extension", isLeft ? "GET" : "POST");

  delay(1 << random(10)); // This delay does not break the communication
  if (isLeft) {
    // delay(1000);
    setLed(1, 0);
    while (!BitBang::receive(msgLen, 1000)) {
      DEBUG_INFO("Retry");
    }
    msg = BitBang::GetValue();
  } else {
    setLed(1, 0);
    while (!BitBang::send(loopIndex, msgLen, 1200)) {
      DEBUG_INFO("Retry");
    }
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
