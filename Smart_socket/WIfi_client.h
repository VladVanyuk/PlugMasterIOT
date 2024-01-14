
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "Relay.h"

#include "Relay.h"

// Replace with your network credentials

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "Relay.h"

// Replace with your network credentials
const char* ssid = "shotytam";
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
