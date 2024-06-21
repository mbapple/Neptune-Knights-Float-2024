#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <AsyncTCP.h>
#include "RTCtime.h"
#include "floatCode.h"
#include "WebServer.h"


const char *ssid = "Neptune Knights Float";
const char *password = "123456789";

//unsigned long last_print_time = millis();

AsyncWebServer serialServer(80);
//WiFiServer serialServer(80);
//WiFiServer webServer(81);

TaskHandle_t deployCode;

// Variable to store the HTTP request
//String header;

/*
void setupWebServer() {
  WiFi.softAP(ssid, password);
  Serial.begin(115200);
  webServer.begin();
}*/


void setupWebSerial() {
  /*
  WiFi.softAP(ssid, password);
  WebSerial.begin(&serialServer);   
  //WebSerial.msgCallback(recvMsg);
  serialServer.begin(); */

  
  WiFi.softAP(ssid, password);
  // Once connected, print IP
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  serialServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is WebSerial demo. You can access webserial interface at http://" + WiFi.softAPIP().toString() + "/webserial");
  });

  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&serialServer);

  /* Attach Message Callback */
  WebSerial.onMessage([&](uint8_t *data, size_t len) {/*
    Serial.printf("Received %u bytes from WebSerial: ", len);
    Serial.write(data, len);
    Serial.println();
    WebSerial.println("Received Data...");*/
    String d = "";
    for(size_t i=0; i < len; i++){
      d += char(data[i]);
    if(d[0] == 'd') {
      displayData();
    }
    if(d[0] == 's') {
          xTaskCreatePinnedToCore (
        deployCodeFunction,
        "deployCode",
        10000,
        NULL,
        1,
        &deployCode,
        1);
    }
    if(d[0] == 'p') {
      displayPacket();
    }
    }
    WebSerial.println(d);
  });


  // Start server
  serialServer.begin();
  /*
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  // Once connected, print IP
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP()); */
}

void deployCodeFunction(void * pvParameters) {
  deployFloat();
}

/*
void webSerialLoop() {
  // Print every 2 seconds (non-blocking)
  if ((unsigned long)(millis() - last_print_time) > 2000) {
    WebSerial.print(F("IP address: "));
    WebSerial.println(WiFi.softAPIP());
    WebSerial.printf("Uptime: %lums\n", millis());
    WebSerial.printf("Free heap: %u\n", ESP.getFreeHeap());
    last_print_time = millis();
  }

  WebSerial.loop();
}*/


/*
void websiteServerLoop() {
  // Print every 2 seconds (non-blocking)
  if ((unsigned long)(millis() - last_print_time) > 2000) {
    WebSerial.print(F("IP address: "));
    WebSerial.println(WiFi.softAPIP());
    WebSerial.printf("Uptime: %lums\n", millis());
    WebSerial.printf("Free heap: %u\n", ESP.getFreeHeap());
    last_print_time = millis();
  }

  WebSerial.loop();
} */

/*
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "deploy") {
    deployFloat(); // depoloy float if "deploy" received
  }
  if (d=="display") {
    displayData(); // display data if "display" received
  }
}*/

/*
void loopWebServer() {
  WiFiClient client = webServer.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    WebSerial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      Serial.println("Client Connected");
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
              WebSerial.println("Float deployd'");
              startFloat(); // start the float, causes the main loop to then deploy the float using core 0
            } else if (header.indexOf("GET /Stop") >= 0) {
              WebSerial.println("Float Stopped");
              stopFloat(); // will cause the floatCode to break out of whichever loop it is in
            } else if (header.indexOf("GET /Display") >= 0) {
              displayData(); // will display the data in the web serial
            } else if (header.indexOf("GET /Packet") >= 0) {
              displayPacket(); // will display the data in the web serial
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
            
            // Button to single packet of data
            client.println("<p>Display Packet</p>");
            client.println("<p><a href=\"/Packet\"><button class=\"button\">DISPLAY CURRENT PACKET</button></a></p>");
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
    WebSerial.println("Client disconnected.");
    WebSerial.println("");
  }
}*/