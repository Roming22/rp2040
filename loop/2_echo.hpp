// Echo the loop value back and forth to the boards
// to check for errors during transmission
void loop_2_echo() {
  static unsigned int loopIndex = 0;
  static unsigned int msgLen = 32;
  static unsigned int msg = 1;
  static unsigned int badValues = 0;
  Pixels &pixels = *Pixels::get();

  Serial.println("");
  Serial.print("## Echo Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
  } else {
    Serial.println(" [Extension]");
  }

  // Receive GO
  if (!isLeft || loopIndex > 1) {
    Serial.println("GET");
    msg = BitBang::receiveData(msgLen) + isLeft;
  }
  if (loopIndex != msg) {
    Serial.println("");
    Serial.println("");
    Serial.print("!!! Bad value: ");
    Serial.print(++badValues);
    Serial.println(" !!!");
    delay(2000);
    if (!isLeft) {
      loopIndex = msg;
    }
  }
  Serial.println("");
  Serial.println("BLINK");
  if (loopIndex % 500 == 0) {
    Serial.print("Bad value check: ");
    Serial.print(badValues);
    pixels.fill(pixels.Color(255 * (badValues > 0), 255 * (badValues == 0), 0));
    pixels.show();
    delay(2000);
  }
  blinkLeds(10);

  // Send GO
  Serial.println("");
  Serial.println("POST");
  BitBang::sendData(loopIndex, msgLen);
}
