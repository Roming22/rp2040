// Blink both boards in sync, and exchange data to synchronize the loopIndex
void loop_4_data() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 0;
  static unsigned int inError = 0;

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);

  if (isLeft) {
    Serial.println(" [Controller]");
    Serial.println("POST Value");
    BitBang::send(loopIndex, msgLen);
  } else {
    Serial.println(" [Extension]");
    Serial.println("GET Value");
    msg = BitBang::receive(msgLen);
  }

  if (loopIndex % 5000 == 42) {
    Serial.print("Error count: ");
    Serial.println(inError);

    if (inError) {
      error(5000);
    }
    blinkLeds(3000);
  }

  // Send GO
  Serial.println("");
  if (isLeft) {
    Serial.println("GET Value");
    msg = BitBang::receive(msgLen);
  } else {
    Serial.println("POST Value");
    BitBang::send(loopIndex, msgLen);
  }

  if (msg != loopIndex) {
    ++inError;
    Serial.println("");
    Serial.println("");
    Serial.println("!!! Bad value !!!");
    Serial.print(msg);
    Serial.print("!=");
    Serial.println(loopIndex);
    Serial.print("Error count: ");
    Serial.println(inError);
    error(10000);
    if (!isLeft) {
      loopIndex = msg;
    }
  }
}
