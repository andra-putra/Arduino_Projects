// Using Arduiono nano every, MPU6050 gyro module, and two 8x8 LED matrixes


#include <LedControl.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>
#include <Wire.h>

MPU6050 gyro(Wire);

int PIN_DATAIN = 5;
int PIN_CLK = 6;
int PIN_LOAD = 4;

LedControl lc = LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, 2);  // The '2' indicates two matrices are chained.

float accX, accY, accZ;

void setup() {
  // put your setup code here, to run once:
  // Wake up the max7219
  lc.shutdown(0, false);
  lc.shutdown(1, false);

  // Set the brightness. Value can be between 0 and 15
  lc.setIntensity(0, 1);
  lc.setIntensity(1, 1);

  // Clear the display
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  Serial.begin(9600);
  Wire.begin();

  gyro.begin();

  gyro.update();

  if (gyro.getAccX() == 0 && gyro.getAccY() == 0 && gyro.getAccZ() == 0) {
    Serial.println("MPU6050 Connection failed or device is perfectly still");
  } else {
    Serial.println("MPU6050 Connection successful");
  }
}

int currentRow = 3;
int currentCol = 3;
float moveSens = 0.1;

// Move functions still very basic. TODO: Acceleration and dynamic speed
void moveRight(int &row) {
  row--;
}

void moveLeft(int &row) {
  row++;
}

void moveUp(int &col) {
  col--;
}

void moveDown(int &col) {
  col++;
}

void updateLED() {
  // Checks Y axis
  if (accY >= moveSens && currentRow > 0) {
    moveRight(currentRow);
  } else if (accY <= -moveSens && currentRow < 7) {
    moveLeft(currentRow);
  }

  // Checks Z axis
  if (accZ >= moveSens && currentCol < 7) {
    moveDown(currentCol);
  } else if (accZ <= -moveSens && currentCol > 0) {
    moveUp(currentCol);
  }

  lc.setLed(0, currentRow, currentCol, true);
  delay(50);
  lc.setLed(0, currentRow, currentCol, false);
}

void printSerial() {
  Serial.print("AccX: ");
  Serial.println(accX);
  Serial.print("AccY: ");
  Serial.println(accY);
  Serial.print("AccZ: ");
  Serial.println(accZ);
  Serial.println("---------------");
  Serial.print("Current row: ");
  Serial.println(currentRow);
  Serial.print("Current col: ");
  Serial.println(currentCol);
  Serial.println();
}

unsigned long previousMillisLED = 0;     // Store when you last updated the LED matrix
unsigned long previousMillisSerial = 0;  // Store when you last printed to the Serial console

const long intervalLED = 50;      // The LED matrix updates every 100ms
const long intervalSerial = 1000;  // The Serial console prints every 1000ms


void loop() {
  unsigned long currentMillis = millis();

  gyro.update();

  accX = gyro.getAccX();
  accY = gyro.getAccY();
  accZ = gyro.getAccZ();

  // Update LED matrix every 100ms
  if (currentMillis - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillis;
    updateLED();
  }

  // Print to Serial every 1000ms
  if (currentMillis - previousMillisSerial >= intervalSerial) {
    previousMillisSerial = currentMillis;
    printSerial();
  }
}
