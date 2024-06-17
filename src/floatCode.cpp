/*
handles the control of the float's servo and the data gathered by the float
*/

#include <ESP32_FastPWM.h>
#include "RTCtime.h"
#include "floatCode.h"
#include "WebServer.h"

#define servoPin 16
#define depthSensorPin 17 // 
#define depthMultiplier 5.0 // what to multiply the analog read value by
#define dataDelay 1000 // milliseconds between data collections
#define servoSpeed 10 // added/subtracted to 50 to get the duty cycle of the servo
#define movementTime 100 // seconds to move servo completely
// TODO: tune these values

ESP32_FAST_PWM* servo;
int counter = 0;

const int ROW = 100;
const int COL = 2;

bool float_Deployed = false;



String data[ROW][COL];

// sets up the servo object for the float
void setupFloat() {
  servo = new ESP32_FAST_PWM(servoPin, 500, 0);

  pinMode(servoPin, OUTPUT);
  pinMode(depthSensorPin, INPUT);
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
    returnFloat(timeInSeconds());
  }
  if(floatDeployed() == true) { // recall deployFloat if floatDeployed true
    logData();
    deployFloat();
  }
  float_Deployed = true;
  delay(dataDelay);
}

void returnFloat(int returnStartTime) { // breaks when floatDeployed == false
  if(timeInSeconds() - returnStartTime < movementTime) { // subtract the time that the return started
    servo->setPWM(servoPin, 50, 50 - servoSpeed); // sets the servo to come up
  }
  else {
    servo->setPWM(servoPin, 50, 50); // sets the servo to stop
  }
  delay(dataDelay);
  if(floatDeployed() == true) { // continue logging data until float no longer deployed
    returnFloat(returnStartTime);
  }
}

void stopFloat() {
    float_Deployed = false;
}

// logs new data
void logData() {
  if(counter <= ROW) { // if there is room in the array, log more data
    data[counter][0] = getRTCTime();
    data[counter][1] = String(convertSensorValue());
    counter++;
  }
}

// outputs the actual depth value from the depth sensor
float convertSensorValue() {
  return float(analogRead(depthSensorPin)) * depthMultiplier;
}

// prints the data to the web serial in a format to be copied and pasted into .csv
void displayData() {
  WebSerial.println();
  WebSerial.println("Data:");
  for(int i = 0; i << ROW; i++) {
    WebSerial.print(data[i][0]);
    WebSerial.print("");
    WebSerial.print(data[i][1]);
    WebSerial.println(",");
  }
}

bool floatDeployed() {
    return float_Deployed;
}