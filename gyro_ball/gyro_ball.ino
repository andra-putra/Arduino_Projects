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

// Ball starting position and movement sens
int currentRow = 7;
int currentCol = 0;
float moveSens = 0.1;

// Defining walls for the LED matrix
// TODO: Integrate matrix for ball movement too, including edge checks etc.
#define WALL 1
#define EMPTY 0
int matrix[8][8] = {
  { EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, WALL, EMPTY },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, WALL, EMPTY },
  { EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY },
  { EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY }
};

unsigned long previousMillisLED = 0;     // Store when you last updated the LED matrix
unsigned long previousMillisSerial = 0;  // Store when you last printed to the Serial console

const long intervalLED = 50;       // The LED matrix updates every 100ms
const long intervalSerial = 1000;  // The Serial console prints every 1000ms
const long blinkInterval = 500;    // Ball blinking interval

unsigned long prevMillisBlink = 0;
bool ballLedState = true;  // True = ball on, false = ball off


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



// Move functions still very basic.
// TODO: Acceleration and dynamic speed
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
  if (accY >= moveSens && currentRow > 0 && matrix[currentRow - 1][currentCol] != WALL) {
    moveRight(currentRow);
  } else if (accY <= -moveSens && currentRow < 7 && matrix[currentRow + 1][currentCol] != WALL) {
    moveLeft(currentRow);
  }

  // Checks Z axis
  if (accZ >= moveSens && currentCol < 7 && matrix[currentRow][currentCol + 1] != WALL) {
    moveDown(currentCol);
  } else if (accZ <= -moveSens && currentCol > 0 && matrix[currentRow][currentCol - 1] != WALL) {
    moveUp(currentCol);
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i == currentRow && j == currentCol) {
        lc.setLed(0, i, j, ballLedState);  // Ball position
        // delay(250);
        // lc.setLed(0, i, j, false);
      } else if (matrix[i][j] == WALL) {
        lc.setLed(0, i, j, true);  // Wall position
      } else {
        lc.setLed(0, i, j, false);  // Empty
      }
    }
  }
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



void loop() {
  unsigned long currentMillis = millis();

  gyro.update();

  accX = gyro.getAccX();
  accY = gyro.getAccY();
  accZ = gyro.getAccZ();

  // Updates matrix LED etc.
  if (currentMillis - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillis;
    updateLED();
  }

  // Print to Serial every 1000ms
  // if (currentMillis - previousMillisSerial >= intervalSerial) {
  //   previousMillisSerial = currentMillis;
  //   printSerial();
  // }

  // Manage LED blinking for ball
  if (currentMillis - prevMillisBlink >= blinkInterval) {
    prevMillisBlink = currentMillis;
    ballLedState = !ballLedState;  // Toggle LED state
    lc.setLed(0, currentRow, currentCol, ballLedState);
  }
}
