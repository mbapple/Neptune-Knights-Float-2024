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
  
  
  // main loop controls the float on core 0
  xTaskCreatePinnedToCore (
    mainLoopCode,
    "mainLoop",
    10000,
    NULL,
    1,
    &mainLoop,
    0); 

  // webserver runs on core 1
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
  setupWebSerial();
  for(;;) {
    if(floatDeployed() == true) {
      //resetTime(); if you want to reset time every deployment
      deployFloat();
    }
  }
} 

void webServerLoopCode(void * pvParameters) {
  for(;;) {
    loopWebServer();
  }
}