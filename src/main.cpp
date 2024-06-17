/*
Matthew Barrs
Neptune Knights Float 2024
*/

#include <Arduino.h>
#include "WebServer.h"
#include "RTCtime.h"
#include "floatCode.h"


// function prototypes
void mainLoopCode(void * pvParameters);
void webServerLoopCode(void * pvParameters);

TaskHandle_t mainLoop;
TaskHandle_t webServerCode;

// set up the ESp32
void setup() {
  // set up an access point from the ESP32
  Serial.begin(115200);
  setupWebServer();
  setupRTCTime();
  setupFloat();

  // setup up the tasks pinned to certain cores
  xTaskCreatePinnedToCore (
    mainLoopCode,
    "mainLoop",
    10000,
    NULL,
    1,
    &mainLoop,
    0);

  xTaskCreatePinnedToCore (
    webServerLoopCode,
    "webServer",
    10000,
    NULL,
    1,
    &webServerCode,
    1);
}

void loop() {

}
  
void mainLoopCode(void * pvParameters) {
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
  for(;;) {
    WebSerial.println(getRTCTime());
    delay(1000);
  }
}

void webServerLoopCode(void * pvParameters) {
  for(;;) {
    loopWebServer();
  }
}