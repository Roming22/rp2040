// Blink both board in sync, with each board having a random delay
void loop_3_sync() {
  static unsigned int loopIndex = 0;
  static Pixels &pixels = *Pixels::get();

  Serial.println("");
  Serial.print("## Loop ");
  Serial.print(++loopIndex);
  if (isLeft) {
    Serial.println(" [Controller]");
    // Send GO
    BitBang::sendSync();
    Serial.println("Sent Go");
  } else {
    Serial.println(" [Extension]");
    // Receive GO
    BitBang::receiveSync();
    Serial.println("Got Go");
  }

  delayMicroseconds(10 + 10 * random(10));

  if (loopIndex % 10000 == 42) {
    Serial.print("Value check: ");
    blinkLeds(3000);
  }
}
