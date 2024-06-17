/*
handles the control of the float's servo and the data gathered by the float
*/

#include <ESP32_FastPWM.h>
#include "RTCtime.h"
#include "floatCode.h"
#include "WebServer.h"

//TODO: change the team number to the team number of the group, add it to the display function
//PN01 1:51:40 UTC 9.8 kpa 1.00 meters
String TeamNumber = "PN07";
#define servoPin 16
#define depthSensorPin 17 // 
//TODO: calculate multiplier to convert to Pa
#define pressureMultiplier 2704 // what to multiply the analog read value by to convert to pressure
//#define depthMultiplier 1/9806.5 // what to multiply the pascals value by to convert to meters
#define dataDelay 5000 // milliseconds between data collections
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
  if(floatDeployed() == true) { // recall deployFloat if floatDeployed still true
    logData();
    deployFloat();
  }
  float_Deployed = true;
  delay(dataDelay);
}

// returns the float in a recursive loop
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

// sets the float_Deployed variable to true
void startFloat() {
    float_Deployed = true;
}

// sets the float_Deployed variable to false, causing the recursvie loop to be broken
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

// outputs the actual pressure value from the sensor
float convertSensorValue() {
  return float(analogRead(depthSensorPin)) * pressureMultiplier;
}

// prints the data to the web serial in a format to be copied and pasted into .csv
void displayData() {
  WebSerial.println();
  WebSerial.println("Data:");
  for(int i = 0; i << ROW; i++) {
    WebSerial.print(TeamNumber + ",");
    WebSerial.print(data[i][0] + ",");
    WebSerial.print(data[i][1] + ",");
    WebSerial.print(pressureToDepth(data[i][1].toFloat()));
    WebSerial.println(",");
  }
}

// displays single packet of current information
void displayPacket() {
  WebSerial.print(TeamNumber + ",");
  WebSerial.print(getRTCTime() + ",");
  WebSerial.print(convertSensorValue());
  WebSerial.print(",");
  WebSerial.print(pressureToDepth(convertSensorValue()));
  WebSerial.print(",");
  WebSerial.println();
}

// returns whether the float is deployed
bool floatDeployed() {
    return float_Deployed;
}

float pressureToDepth(float pressure) {
  return (pressure - 101325) / 9806.5;
}