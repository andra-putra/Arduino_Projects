#include <LedControl.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>

int PIN_DATAIN = 5;
int PIN_CLK = 4;
int PIN_LOAD = 6;

LedControl lc = LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, 2);  // The '2' indicates two matrices are chained.


void setup() {
  // put your setup code here, to run once:
  lc.clearDisplay(0);
  lc.clearDisplay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  lc.setLed(0, 4, 4, false);
  delay(500);
  lc.setLed(0, 4, 4, true);
  delay(500);
}
