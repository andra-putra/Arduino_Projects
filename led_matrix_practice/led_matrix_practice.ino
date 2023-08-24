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

int counter = 0;

void loop() {
  counter = counter + 1;

  for (int i = 0; i < 4; i++) {

    lc.setLed(0, i, i, true);
    lc.setLed(1, 7 - i, i, true);
    delay(100);
    lc.setLed(0, i, i, false);
    lc.setLed(1, 7 - i, i, false);
    // delay(100);
  }


  for (int i = 4; i < 8; i++) {

    lc.setLed(0, 7 - i, i, true);
    lc.setLed(1, i, i, true);
    delay(100);
    lc.setLed(0, 7 - i, i, false);
    lc.setLed(1, i, i, false);
    // delay(100);
  }



  // delay(1000);
  // lc.clearDisplay(0);
  // lc.clearDisplay(1);
}
