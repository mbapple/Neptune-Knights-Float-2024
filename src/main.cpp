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
  
  /*
  // setup up the tasks pinned to certain cores
  xTaskCreatePinnedToCore (
    mainLoopCode,
    "mainLoop",
    10000,
    NULL,
    1,
    &mainLoop,
    0); */

  // webserver runs on core 2
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

 /* 
void mainLoopCode(void * pvParameters) {
  for(;;) {
    WebSerial.println(getRTCTime());
    delay(1000);
  }
} */

void webServerLoopCode(void * pvParameters) {
  for(;;) {
    loopWebServer();
  }
}