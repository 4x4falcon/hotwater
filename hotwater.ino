/*
 * Demonstrate using an http server and an HTML form to control an LED.
 * The http server runs on the ESP8266.
 *
 * Connect to "http://esp8266WebForm.local" or "http://<IP address>"
 * to bring up an HTML form to control the LED connected GPIO#0. This works
 * for the Adafruit ESP8266 HUZZAH but the LED may be on a different pin on
 * other breakout boards.
 *
 * Imperatives to turn the LED on/off using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform.local/ledon
 * $ wget http://esp8266webform.local/ledoff
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

ESP8266WebServer server(80);

int ledPin = BUILTIN_LED;

#include "settings.h"
#include "html_page.h"
#include "myfunctions.h"


void setup(void)
{
  pinMode(ledPin, OUTPUT);
  writeLed(false);
  pinMode(relayPin, OUTPUT);
  writeRelay(false);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin(espName, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ledon", handleLedOn);
  server.on("/ledoff", handleLedOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://");
  Serial.print(espName);
  Serial.print(".local or http://");
  Serial.println(WiFi.localIP());
}


void loop(void)
{
  server.handleClient();

//  Serial.println(loopStart);

  if (loopStart != 0) {
    if (millis() > loopStart + oneDay) {
      loopStart = 0;
      handleLedOff();
      Serial.println("Turning OFF");
    }
  }
}

