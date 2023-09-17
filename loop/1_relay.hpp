// Blink in alternatively by passing a GO message
void loop_1_relay() {
  static unsigned int loopIndex = 0;

  Serial.println("");
  Serial.print("## Relay Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
  } else {
    Serial.println(" [Extension]");
  }

  // Wait for go
  if (!isLeft || loopIndex > 1) {
    BitBang::receiveSync();
  }
  if (loopIndex % 500 == 0) {
    delay(2000);
  }

  blinkLeds(100);

  // Send go
  BitBang::sendSync();
}
