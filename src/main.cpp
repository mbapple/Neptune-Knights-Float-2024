/*
Matthew Barrs
Neptune Knights Float 2024
*/

#include <Arduino.h>
#include "RTCtime.h"
#include "WebServer.h"

#define servoPin 16
#define depthSensorPin 17 // 
#define depthMultiplier 100 // what to multiply the analog read value by
#define dataDelay 1000 // milliseconds between data collections
#define servoSpeed 10 // 10-90

const int ROW = 100;
const int COL = 2;

int data[ROW][COL];

void setup() {
  // set up an access point from the ESP32
  Serial.begin(115200);
  setupWebServer();
  setupRTCTime();

  pinMode(servoPin, OUTPUT);
  pinMode(depthSensorPin, INPUT);
}
  
void loop() {
  
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