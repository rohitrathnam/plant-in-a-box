#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <WiFiUdp.h>

#include "resources.h"
#include "DHT.h"

MDNSResponder mdns;

ESP8266WebServer server(80);

IPAddress timeServerIP; // time.nist.gov NTP server address

WiFiUDP udp;

DHT dht(DHTPin, DHTTYPE);

void setup(void)
{
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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

  if (mdns.begin("plantinabox", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/control", handleManualCtrl);
  server.on("/time", handleTime);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://plantinabox.local or http://");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  while (!getTime());
  Serial.println("Ready!");
  dht.begin();
}

void loop(void)
{
  updateTime();
  checkStats();
  server.handleClient();
}
