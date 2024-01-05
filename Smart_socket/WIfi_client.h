<<<<<<< Updated upstream
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "shotytam";
=======
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "Relay.h"

// Replace with your network credentials
const char* ssid = "shotytam";
>>>>>>> Stashed changes
const char* password = "shotytambar";

const int PORT = 80;

WiFiServer server(PORT);

void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // OTA setup
  ArduinoOTA.begin();
  ArduinoOTA.setHostname("ESP8266_OTA"); // Set the hostname for OTA
  ArduinoOTA.begin();
}

void wifi_client() {
  ArduinoOTA.handle(); // Handle OTA

<<<<<<< Updated upstream
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {            // if the byte is a newline character
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0) {
                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();
                    
                   if (header.indexOf("GET /on") >= 0) {
                   Serial.println("GPIO on");
                   state = ON; // Використовуйте константу ON
                   digitalWrite(LED_BUILTIN, HIGH);
                   } else if (header.indexOf("GET /off") >= 0) {
                   Serial.println("GPIO off");
                    state = OFF; // Використовуйте константу OFF
                     digitalWrite(LED_BUILTIN, LOW);
                     }

                    
                    // Display the HTML web page
                    client.println("<!DOCTYPE html><html>");
                    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                    client.println("<link rel=\"icon\" href=\"data:,\">");
                    // CSS to style the on/off button 
                    // Feel free to change the background-color and font-size attributes to fit your preferences
                    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                    client.println(".button { background-color: #800000; border: none; color: white; padding: 16px 40px;");
                    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                    client.println(".button2 {background-color: #195B6A;}</style></head>");
                    
                    // Web Page Heading
                    client.println("<body><h1>ESP8266 Web Controll LED</h1>");
                    
                    // Display current state, and ON/OFF button for the GPIO  
                    client.print("<p>GPIO - State ");
                    if (state == OFF) {
                      client.print("ON");
                      } else {
                      client.print("OFF");
                        }
                      client.println("</p>");
                    // If the outputState is off, it displays the ON button       
                    if (state == OFF) {
                        client.println("<p><a href=\"/on\"><button class=\"button\">OFF</button></a></p>");
                    } else {
                        client.println("<p><a href=\"/off\"><button class=\"button button2\">ON</button></a></p>");
                    } 
                    
                    client.println("</body></html>");
                    
                    // The HTTP response ends with another blank line
                    client.println();
                    // Break out of the while loop
                    break;
                } else { // if you got a newline, then clear currentLine
                    currentLine = "";
                }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;    // add it to the end of the currentLine
=======
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            String request = client.readStringUntil('\r');
            if (request.indexOf("GET /on") != -1) {
              Serial.println("RELAY on");
              relay_state = ON;
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (request.indexOf("GET /off") != -1) {
              Serial.println("RELAY off");
              relay_state = OFF;
              digitalWrite(LED_BUILTIN, LOW);
>>>>>>> Stashed changes
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body><h1>SMART SOCKET Web Control</h1><p>RELAY - State ");
            
            if (relay_state == OFF) {
              client.print("ON");
            } else {
              client.print("OFF");
            }
            
            client.println("</p>");
            
            if (relay_state == OFF) {
              client.println("<p><a href=\"/on\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/off\"><button class=\"button button2\">ON</button></a></p>");
            } 
            
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
