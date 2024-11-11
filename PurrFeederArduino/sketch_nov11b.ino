#include <WiFi.h>
#include <WebServer.h>
#include "page.h"

#define AP_SSID "PurrFeeder_AP"
#define AP_PASS "purrfeeder"

IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
IPAddress Actual_IP;

WebServer server(80);

void SendWebsite()
{
  server.send(200, "text/html", PAGE_MAIN);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: "); Serial.println(Actual_IP);  
  server.on("/", SendWebsite);
  server.begin();
  
  server.send(200, "text/html", PAGE_MAIN);
}


void loop() {
  server.handleClient();
}