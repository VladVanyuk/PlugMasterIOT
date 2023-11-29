#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncButton.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>

#define ENC_PIN_R D7
#define ENC_PIN_L D6
#define ENC_PIN_BTN D5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define ON 0
#define OFF 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
EncButton eb(ENC_PIN_R, ENC_PIN_L, ENC_PIN_BTN);

// Replace with your network credentials
const char* ssid     = "252";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

bool previousLedState = OFF;
bool screenNeedsUpdate = false;
bool state = OFF;

const long interval = 2000;
unsigned long previousMillis = 0;

void encoder_setup() 
{
 eb.setBtnLevel(LOW);
  eb.setClickTimeout(500);
  eb.setDebTimeout(50);
  eb.setHoldTimeout(600);
  eb.setStepTimeout(200);

  eb.setEncReverse(0);
  eb.setEncType(EB_STEP4_LOW);
  eb.setFastTimeout(30);
  eb.counter = 0;
}

void wifi_setup()
{
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, state);

  encoder_setup();
  wifi_setup();
}

void change_state() {
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

void encoder_handler() {
  eb.tick();
  if (eb.press()) {
    change_state();
  }

  if (eb.click()) {
    Serial.println("click");
  }
}

void led_oled() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Отримати стан світлодіода
    bool currentState = digitalRead(LED_BUILTIN);

    // Перевірити зміну стану світлодіода
    if (currentState != previousLedState) {
      screenNeedsUpdate = true;
      previousLedState = currentState;
    }

    if (screenNeedsUpdate) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      if (currentState == ON) {
        display.println("LED is ON!");
      } else {
        display.println("LED is OFF!");
      }

      display.display();

      screenNeedsUpdate = false;
    }
  }
}

void wifi_client()
{
  WiFiClient client = server.available();   // Listen for incoming clients

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

void loop() {
  encoder_handler();
  led_oled();
  wifi_client();
}
