// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop_5_data() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  DEBUG_INFO("## Loop %d [%s] %s Value", ++loopIndex,
             isLeft ? "Controller" : "Extension", isLeft ? "POST" : "GET");

  if (isLeft) {
    msg = BitBang::receive(msgLen);
  } else {
    BitBang::send(loopIndex, msgLen);
  }

  if (loopIndex % 5000 == 42) {
    Serial.print("Error count: ");
    Serial.println(inError);

    if (inError) {
      error(5000);
    }
    delay(3000);
  }

  Serial.println("");
  DEBUG_INFO("%s Value", ++loopIndex, isLeft ? "Controller" : "Extension",
             isLeft ? "GET" : "POST");
  delay(10);
  if (isLeft) {
    msg = BitBang::receive(msgLen);
  } else {
    BitBang::send(loopIndex, msgLen);
  }

  if (msg != loopIndex) {
    ++inError;
    Serial.println("");
    Serial.println("");
    Serial.println("!!! Bad value !!!");
    DEBUG_ERROR("%d != %d", msg, loopIndex);
    DEBUG_ERROR("Error count: %d", inError);
    error(10000);
    if (!isLeft) {
      loopIndex = msg;
    }
  }
}
