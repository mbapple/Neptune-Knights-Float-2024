#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>


void setupWebServer();
void setupWebSerial();
void loopWebServer();
void webServerLoop();
void webSerialLoop();
void deployCodeFunction(void * pvParameters);