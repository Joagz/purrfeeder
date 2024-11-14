#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <vector>
#include "page.h"
#define AP_SSID "PurrFeeder_AP"
#define AP_PASS "purrfeeder"

IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
IPAddress Actual_IP;

#define FORMAT_LITTLEFS_IF_FAILED true

char TIME_ARR[256];
WebServer server(80);
char* addElementToArray(char* arrayStr, const String& newElement);
std::vector<String> traverseArray(char* arrayStr);

void SendWebsite() {
  server.send(200, "text/html", PAGE_MAIN);
}

void UpdateTime() {
  String time = server.arg("VAL");

  addElementToArray(TIME_ARR, time);
  Serial.println(TIME_ARR);

  server.send(200, "text/html", "updated!");
}

void DeleteTime() {
  String time = server.arg("VAL");

  std::vector<String> time_list = traverseArray(TIME_ARR);
  Serial.println(time);
  auto it = std::find(time_list.begin(), time_list.end(), time);

  if (it != time_list.end()) {
    Serial.println("Element found and erased");

    time_list.erase(it);
  } else {
    Serial.println("Element not found");
  }


  char temp[256];
  for (auto& v : time_list) {
    if (v.length() != 5) continue;
    addElementToArray(temp, v);
  }

  memcpy(TIME_ARR, temp, 256);

  server.send(200, "text/html", "updated!");
}

char* addElementToArray(char* arrayStr, const String& newElement) {
  String arrayString(arrayStr);

  if (arrayString.length() <= 2) {
    arrayString.remove(arrayString.length() - 1);
    arrayString += "[";
    arrayString += "\"" + newElement + "\"";
    arrayString += "]";
  } else {

    arrayString.remove(arrayString.length() - 1);
    arrayString += ", \"" + newElement + "\"";
    arrayString += "]";
  }

  char* updatedArrayStr = new char[arrayString.length() + 1];
  arrayString.toCharArray(updatedArrayStr, arrayString.length() + 1);

  strcpy(arrayStr, updatedArrayStr);

  return updatedArrayStr;
}

std::vector<String> traverseArray(char* arrayStr) {
  std::vector<String> values;
  String arrayString(arrayStr);

  arrayString.remove(0, 1);
  arrayString.remove(arrayString.length() - 1, 1);

  int startIndex = 0;
  while (startIndex < arrayString.length()) {
    int endIndex = arrayString.indexOf("\"", startIndex + 1);
    if (endIndex == -1) break;
    String element = arrayString.substring(startIndex + 1, endIndex);
    values.push_back(element);

    startIndex = endIndex + 1;
  }

  return values;
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(Actual_IP);

  server.on("/time", UpdateTime);
  server.on("/delete", DeleteTime);
  server.on("/", SendWebsite);
  server.begin();
}


void loop() {
  server.handleClient();
}
