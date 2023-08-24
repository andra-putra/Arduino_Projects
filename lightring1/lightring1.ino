int ledOne = 2;
int ledTwo = 3;
int ledThree = 4;
int ledFour = 5;

int brightness = 0; // Goes from 0 - 255



void setup() {
  // put your setup code here, to run once:
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
  pinMode(ledFour, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  turnOnLED(ledOne, "LED One");
  turnOnLED(ledTwo, "LED Two");
  turnOnLED(ledThree, "LED Three");
  turnOnLED(ledFour, "LED Four");
}

void turnOnLED(int ledPin, String ledName) {
  analogWrite(ledPin, 1);
  // Serial.println(ledName);
  delay(240);
  analogWrite(ledPin, 0);
  delay(10);
}


