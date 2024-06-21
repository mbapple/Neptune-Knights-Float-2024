/*
handles the control of the float's servo and the data gathered by the float
*/

#include <ESP32Servo.h>
#include "RTCtime.h"
#include "floatCode.h"
#include "WebServer.h"

//TODO: change the team number to the team number of the group, add it to the display function
//PN01 1:51:40 UTC 9.8 kpa 1.00 meters
String TeamNumber = "PN07";
#define SERVO_PIN 22
#define DEPTH_SENSOR_PIN 32 // 
//TODO: calculate multiplier to convert to Pa
#define PRESSURE_MULTIPLIER 2704 // what to multiply the analog read value by to convert to pressure
//#define depthMultiplier 1/9806.5 // what to multiply the pascals value by to convert to meters
#define DATA_DELAY 5000 // milliseconds between data collections
#define SERVO_SPEED 0 // added/subtracted to 50 to get the duty cycle of the servo
#define MOVEMENT_TIME 10 // seconds to move servo completely
// TODO: tune these values
#define FULL_TIME 30 // seconds to wait at the bottom

Servo servo;

int counter = 0;

const int ROW = 5;
const int COL = 2;

//bool float_Deployed = false;



//int data[][COL] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
int data[ROW][COL];

// sets up the servo object for the float
void setupFloat() {
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.attach(SERVO_PIN, 500, 2500); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// differ ent servos may require different min/max settings
	// for an accurate 0 to 180 sweep

  //pinMode(SERVO_PIN, OUTPUT);
  pinMode(DEPTH_SENSOR_PIN, INPUT);
}

void deployFloat() {
  WebSerial.print("deployFloat() called");
  counter = 0; // reset data counter
  int startTime = timeInSeconds();
  while(timeInSeconds() - startTime < MOVEMENT_TIME) { // makes the float sink
    servo.write(90 - SERVO_SPEED);
    logData();
    //WebSerial.println("Float Going Down");
    delay(DATA_DELAY);
  }
  while(timeInSeconds() - startTime < FULL_TIME + MOVEMENT_TIME) { // how long to wait for float to sink completely
    servo.write(90);
    logData();
    //WebSerial.println("Float Holding");
    delay(DATA_DELAY);
  }
  while(timeInSeconds() - startTime < MOVEMENT_TIME * 2 + FULL_TIME) { // extends float, causing float to resurface
    servo.write(90 + SERVO_SPEED);
    logData();
    //WebSerial.println("Float Going Up");
    delay(DATA_DELAY);
  }
  servo.write(90); // stops the float
  WebSerial.println("Float Returned!");
}

/*
// deploys float in a recursive loop
void deployFloat() {
  WebSerial.println("deployFloat() called");
  if(timeInSeconds() < MOVEMENT_TIME) {
    servo->setPWM(SERVO_PIN, 50, 50 + SERVO_SPEED); // sets the servo to go down
    //WebSerial.println("Servo Speed Set");
  }
  else {
    servo->setPWM(SERVO_PIN, 50, 50); // sets the servo to stop
  }
  if(false) { // TODO: add a condition to stop the loop
    returnFloat(timeInSeconds());
  }
  if(floatDeployed() == true) { // recall deployFloat if floatDeployed still true
    logData();
    deployFloat();
  }
  float_Deployed = true;
  delay(DATA_DELAY);
}

// returns the float in a recursive loop
void returnFloat(int returnStartTime) { // breaks when floatDeployed == false
  if(timeInSeconds() - returnStartTime < MOVEMENT_TIME) { // subtract the time that the return started
    servo->setPWM(SERVO_PIN, 50, 50 - SERVO_SPEED); // sets the servo to come up
  }
  else {
    servo->setPWM(SERVO_PIN, 50, 50); // sets the servo to stop
  }
  delay(DATA_DELAY);
  if(floatDeployed() == true) { // continue logging data until float no longer deployed
    returnFloat(returnStartTime);
  }
}
*/

/*
// sets the float_Deployed variable to true
void startFloat() {
    float_Deployed = true;
}

// sets the float_Deployed variable to false, causing the recursvie loop to be broken
void stopFloat() {
    float_Deployed = false;
}
*/

// logs new data
void logData() {
  if(counter < ROW) { // if there is room in the array, log more data
    data[counter][0] = timeInSeconds();
    WebSerial.println("Data logged at: " + timeInSeconds()  + analogRead(DEPTH_SENSOR_PIN));
    data[counter][1] = analogRead(DEPTH_SENSOR_PIN);
    WebSerial.println(data[counter][0] + " " + data[counter][1]);
    counter++;
  }
  
}

// outputs the actual pressure value from ta sensor reading
float convertSensorValue(int sensorValue) {
  return float(sensorValue) * PRESSURE_MULTIPLIER;
}

// prints the data to the web serial in a format to be copied and pasted into .csv
void displayData() {
  WebSerial.println();
  WebSerial.println("Data:");
  for(int i = 0; i << ROW; i++) {
    WebSerial.println("Data being printed");
    WebSerial.print(TeamNumber + ",");
    WebSerial.print(convertSecondsToTime(data[i][0]) + ",");
    WebSerial.print(String(convertSensorValue(data[i][1])) + ",");
    WebSerial.print(pressureToDepth(convertSensorValue(data[i][1])));
    WebSerial.println(",");
  }
}

// displays single packet of current information
void displayPacket() {
  WebSerial.println();
  WebSerial.print((TeamNumber + "," + getRTCTime() + "," + convertSensorValue(analogRead(DEPTH_SENSOR_PIN)) + "," + pressureToDepth(convertSensorValue(analogRead(DEPTH_SENSOR_PIN)))) + ",");
  WebSerial.println();
}
/*
// returns whether the float is deployed
bool floatDeployed() {
    return float_Deployed;
}*/

// converts pressure in pascals to depth in meters
float pressureToDepth(float pressure) {
  return (pressure - 101325) / 9806.5;
}