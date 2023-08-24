

void turnOnLED(int ledPin, int brightness) {
  analogWrite(ledPin, brightness);
}



void turnOffLED(int ledPin) {
  analogWrite(ledPin, 0);
}