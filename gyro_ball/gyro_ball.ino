// Libraries
#include <LedControl.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>
#include <Wire.h>

// Constants and Macros
#define WALL 1
#define EMPTY 0

// Global Variables: Pins
int PIN_DATAIN = 5;
int PIN_CLK = 6;
int PIN_LOAD = 4;

// Global Variables: Objects
MPU6050 gyro(Wire);
LedControl lc = LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, 2);  // Chained matrices

// Global Variables: Acceleration and Position
float accX, accY, accZ;
int currentRow = 7;  // Ball starting position
int currentCol = 0;
float moveSens = 0.1;  // Movement sensitivity

// Maze Matrix
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

// Timers and Intervals
unsigned long previousMillisLED = 0;
unsigned long previousMillisSerial = 0;
unsigned long prevMillisBlink = 0;
const long intervalLED = 50;
const long intervalSerial = 1000;
const long blinkInterval = 500;

// Ball LED Blink State
bool ballLedState = true;

// Function Prototypes
void moveRight(int &row);
void moveLeft(int &row);
void moveUp(int &col);
void moveDown(int &col);
void updateLED();
void printSerial();

// Initialization
void setup() {
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.setIntensity(0, 1);
  lc.setIntensity(1, 1);
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

// Main Loop
void loop() {
  unsigned long currentMillis = millis();

  gyro.update();
  accX = gyro.getAccX();
  accY = gyro.getAccY();
  accZ = gyro.getAccZ();

  if (currentMillis - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillis;
    updateLED();
  }

  if (currentMillis - prevMillisBlink >= blinkInterval) {
    prevMillisBlink = currentMillis;
    ballLedState = !ballLedState;  // Toggle ball LED state
    lc.setLed(0, currentRow, currentCol, ballLedState);
  }
}

// Function Definitions
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


  // LED Update logic
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
  // Serial printing logic for debug
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
