#include <SPI.h>
#include <WiFiNINA.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(3, 4); //rx,tx

// rocker switch right slot (gold) is VCC; middle slot is PIN 8; left slot is GND
// LED short leg to GND; long leg to PIN 13
int switch_pin = 8;
int led = 13;

char ssid[] = "TELUS965C";          //  your network SSID (name)
char pass[] = "6048732315";         //  your network password

int status = WL_IDLE_STATUS;
IPAddress server(74, 125, 115, 105); // Google

// Initialize the client library
WiFiClient client;

void setup() {
  Serial.begin(9600);
    Serial.println("Attempting to connect to WPA network...");
    Serial.print("SSID: ");
    Serial.println(ssid);
  
    status = WiFi.begin(ssid, pass);
    if ( status != WL_CONNECTED) {
      Serial.println("Couldn't get a wifi connection");
      // don't do anything else:
      while(true);
    }
    else {
      Serial.println("Connected to wifi");
      Serial.println("\nStarting connection...");
      // if you get a connection, report back via serial:
      if (client.connect(server, 80)) {
        Serial.println("connected");
        // Make a HTTP request:
        client.println("GET /search?q=arduino HTTP/1.0");
        client.println();
      }
    }
  Serial.println("The GPS Received Signal:");
  ss.begin(9600); // connect gps sensor
  pinMode(switch_pin, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  delay(1000);
  if (digitalRead(switch_pin) == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("ON");
     while (ss.available() > 0) {
    // get the byte data from the GPS
    byte gpsData = ss.read();
    Serial.write(gpsData);
  }
  delay(1000);
  }
  if (digitalRead(switch_pin) == LOW) {
    digitalWrite(led, LOW);
    Serial.println("OFF");
  }
}
