//#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

MPU6050 mpu1;

int ledOne = 2;
int ledTwo = 3;
int ledThree = 4;
int ledFour = 5;

int brightness = 0; // Goes from 0 - 255
int sens = 2500; // Goes from 0 to 16k iirc
int maxSens = 16000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  mpu1.initialize();
  if (mpu1.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }

  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
  pinMode(ledFour, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu1.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Serial.print("Accel X: "); Serial.print(ax);
  // Serial.print(" | Accel Y: "); Serial.print(ay);
  // Serial.print(" | Accel Z: "); Serial.println(az);
  // Serial.println();
  // Serial.println();
  // Serial.println();


  int ledBrightness = 0; // Default brightness value

  // if (ax >= sens) {
  //   turnOnLED(ledFour);
  // } else if (ax <= -sens) {
  //   turnOnLED(ledTwo);
  // } else if (ay >= sens) {
  //   turnOnLED(ledThree);
  // } else if (ay <= -sens) {
  //   turnOnLED(ledOne);
  // } else {
  //   turnOffLED(ledOne);
  //   turnOffLED(ledTwo);
  //   turnOffLED(ledThree);
  //   turnOffLED(ledFour);
  // }

  if (abs(ax) > sens) {
    ledBrightness = map(abs(ax), sens, maxSens, 0, 255);
    if (ax >= sens) {
      turnOnLED(ledFour, ledBrightness);
    } else {
      turnOnLED(ledTwo, ledBrightness);
    }
  } else if (abs(ay) > sens) {
    ledBrightness = map(abs(ay), sens, maxSens, 0, 255);
    if (ay >= sens) {
      turnOnLED(ledThree, ledBrightness);
    } else {
      turnOnLED(ledOne, ledBrightness);
    }
  } else {
    turnOffLED(ledOne);
    turnOffLED(ledTwo);
    turnOffLED(ledThree);
    turnOffLED(ledFour);
  }

  delay(100);
  
}
