/*
Matthew Barrs
Neptune Knights Float 2024
*/

#include <Arduino.h>
#include "RTCtime.h"
#include "WebServer.h"
#include <ESP32_FastPWM.h>


#define servoPin 16
#define depthSensorPin 17 // 
#define depthMultiplier 100 // what to multiply the analog read value by
#define dataDelay 1000 // milliseconds between data collections
#define servoSpeed 50 // 10-90

const int ROW = 100;
const int COL = 2;

int data[ROW][COL];
ESP32_FAST_PWM* servo;

void setup() {
  // set up an access point from the ESP32
  Serial.begin(115200);
  setupWebServer();
  setupRTCTime();

  pinMode(servoPin, OUTPUT);
  pinMode(depthSensorPin, INPUT);

  servo = new ESP32_FAST_PWM(servoPin, 500, 0);
}
  
void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 100; angle++) {
    servo->setPWM(servoPin, 50, angle);
    delay(servoSpeed);
  }
  
  // Sweep the servo from 180 to 0 degrees
  for (int angle = 100; angle >= 0; angle--) {
    servo->setPWM(servoPin, 50, angle);
    delay(servoSpeed);
  }
}

// recursive loop, deploys float in a recursive loop
void deployFloat() {
  delay(dataDelay);

}

void returnFloat() {

}

float convertSensorValue() {
  return analogRead(depthSensorPin);
}