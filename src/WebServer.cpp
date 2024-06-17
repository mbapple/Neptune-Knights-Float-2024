#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "RTCtime.h"
#include "floatCode.h"


const char *ssid = "Neptune Knights Float";
const char *password = "123456789";

AsyncWebServer serialServer(80);
WiFiServer webServer(81);

// Variable to store the HTTP request
String header;

void setupWebServer() {
  WiFi.softAP(ssid, password);
  
  WebSerial.begin(&serialServer);   
  serialServer.begin();

  webServer.begin();
}

void loopWebServer() {
  WiFiClient client = webServer.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // do stuff if button pushed
            if (header.indexOf("GET /Deploy/NotDeployed") >= 0) {
              resetTime();
              deployFloat();
            } else if (header.indexOf("GET /Stop") >= 0) {
              stopFloat();
            } else if (header.indexOf("GET /Display") >= 0) {
              displayData();
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Neptune Knights Float</h1>");
            
            // Display current state, whether float is deployed
            client.println("<p>Deploy Float</p>");     
            if (!floatDeployed()) {
              client.println("<p><a href=\"/Deploy/NotDeployed\"><button class=\"button\">DEPLOY</button></a></p>");
            } else {
              client.println("<p><a href=\"/Deploy/Deployed\"><button class=\"button button2\">FLOAT DEPLOYED</button></a></p>");
            } 
               
            // Button to stop the float 
            client.println("<p>Stop Float</p>");
            client.println("<p><a href=\"/Stop\"><button class=\"button\">STOP FLOAT</button></a></p>");

            // Button to display data
            client.println("<p>Display Data</p>");
            client.println("<p><a href=\"/Display\"><button class=\"button\">DISPLAY DATA</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}