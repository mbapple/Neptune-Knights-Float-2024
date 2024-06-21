/*
Matthew Barrs
Neptune Knights Float 2024
*/

#include <Arduino.h>
#include "RTCtime.h"
#include "floatCode.h"
#include "WebServer.h"

/*
// function prototypes
void mainLoopCode(void * pvParameters);
void webServerLoopCode(void * pvParameters);

TaskHandle_t mainLoop;
TaskHandle_t webServerCode;
*/

//unsigned long last_print_time = millis();

// set up the ESp32
void setup() {
  Serial.begin(115200);
  // set up an access point from the ESP32
  // Serial.begin(115200);
  setupWebSerial();
  setupRTCTime();
  setupFloat();
  
  /*
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
    */
}

void loop() {
  // Print every 2 seconds (non-blocking)
  /*if ((unsigned long)(millis() - last_print_time) > 2000) {
    WebSerial.print(F("IP address: "));
    WebSerial.println(WiFi.softAPIP());
    WebSerial.printf("Uptime: %lums\n", millis());
    WebSerial.printf("Free heap: %u\n", ESP.getFreeHeap());
    last_print_time = millis();
  }*/
  
  WebSerial.loop();
  //WebSerial.print  delay(100);ln(getRTCTime());
  //WebSerial.println(timeInSeconds());
}

/*
void mainLoopCode(void * pvParameters) {
  
  for(;;) {
    if(floatDeployed() == true) {
      //resetTime(); if you want to reset time every deployment
      deployFloat();
    }
  }
} 

void webServerLoopCode(void * pvParameters) {
  setupWebSerial();
  for(;;) {
    loopWebServer();
  }
}
*/