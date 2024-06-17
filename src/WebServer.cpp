#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>


const char *ssid = "Neptune Knights Float";
const char *password = "123456789";

AsyncWebServer server(80);

void setupWebServer() {
  WiFi.softAP(ssid, password);
  
  WebSerial.begin(&server);   
  server.begin();
}