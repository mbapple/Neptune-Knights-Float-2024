/*
Matthew Barrs
Neptune Knights Float 2024
*/

#include <Arduino.h>
#include "RTCtime.h"
#include "WebServer.h"
#include <ESP32_FastPWM.h>

// function prototypes
void deployFloat();
void returnFloat(int returnStartTime);
void logData();
float convertSensorValue();
void displayData();

#define servoPin 16
#define depthSensorPin 17 // 
#define depthMultiplier 5.0 // what to multiply the analog read value by
#define dataDelay 1000 // milliseconds between data collections
#define servoSpeed 10 // added/subtracted to 50 to get the duty cycle of the servo
#define movementTime 100 // seconds to move servo completely
// TODO: tune these values

const int ROW = 100;
const int COL = 2;

int counter = 0;

String data[ROW][COL];
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
  /*
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
  */
  WebSerial.println(getRTCTime());
  delay(1000);
}



// deploys float in a recursive loop
void deployFloat() {
  if(timeInSeconds() < movementTime) {
    servo->setPWM(servoPin, 50, 50 + servoSpeed); // sets the servo to go down
  }
  else {
    servo->setPWM(servoPin, 50, 50); // sets the servo to stop
  }
  
  if(true) { // TODO: add a condition to stop the loop
    logData();
    deployFloat();
  }
  if(true) { // TODO: add a condition to stop the loop
    returnFloat(timeInSeconds());
  }
  delay(dataDelay);
}

void returnFloat(int returnStartTime) {
  if(timeInSeconds() - returnStartTime < movementTime) { // subtract the time that the return started
    servo->setPWM(servoPin, 50, 50 - servoSpeed); // sets the servo to come up
  }
  else {
    servo->setPWM(servoPin, 50, 50); // sets the servo to stop
  }
  delay(dataDelay);
  if(true) {
    logData();
    returnFloat(returnStartTime);
  }
}

void logData() {
  if(counter << ROW) { // if there is room in the array, log more data
    data[counter][0] = getRTCTime();
    data[counter][1] = String(convertSensorValue());
    counter++;
  }
}

float convertSensorValue() {
  return float(analogRead(depthSensorPin)) * depthMultiplier;
}

void displayData() {

}