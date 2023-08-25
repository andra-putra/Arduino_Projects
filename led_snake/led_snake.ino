// Using Arduiono nano every, MPU6050 gyro module, and two 8x8 LED matrixes


#include <LedControl.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>

int PIN_DATAIN = 5;
int PIN_CLK = 6;
int PIN_LOAD = 4;

LedControl lc = LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, 2);  // The '2' indicates two matrices are chained.


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
}

int delayTime = 100;

void loop() {

  for (int i = 0; i < 8; i += 2) {
    for (int j = 7; j >= 0; j--) {
      lc.setLed(0, j, i, true);
      delay(delayTime);
      lc.setLed(0, j, i, false);
    }
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, j, i+1, true);
      delay(delayTime);
      lc.setLed(0, j, i+1, false);
    }
  }



  // delay(1000);
  // lc.clearDisplay(0);
  // lc.clearDisplay(1);
}
