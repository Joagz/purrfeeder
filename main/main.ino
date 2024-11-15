#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <vector>
#include <WiFiManager.h>
#include "time.h"
#include "page.h"

#define AP_SSID "PurrFeeder_AP"

IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
IPAddress Actual_IP;
const char* ntpServer = "0.south-america.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = -10800;
const int hour_offset = 3;
bool time_toggle = true;

std::vector<String> TIME_LIST;
WebServer server(80);

char* addElementToArray(std::vector<String>& timeList, const String& newElement);
std::vector<String> traverseArray(const String& arrayStr);

void SendWebsite() {
  server.send(200, "text/html", PAGE_MAIN);
}

void GetTimeList() {
  String jsonStr = "[";
  for (size_t i = 0; i < TIME_LIST.size(); i++) {
    jsonStr += "\"" + TIME_LIST[i] + "\"";
    if (i < TIME_LIST.size() - 1) {
      jsonStr += ",";
    }
  }
  jsonStr += "]";
  server.send(200, "application/json", jsonStr);
}

void UpdateTime() {
  String time = server.arg("VAL");

  addElementToArray(TIME_LIST, time);
  Serial.println("Updated time list: ");
  for (const auto& t : TIME_LIST) {
    Serial.println(t);
  }

  server.send(200, "text/html", "updated!");
}

String formatTime(String time) {
  int hour = time.substring(0, time.indexOf(":")).toInt();
  int minute = time.substring(time.indexOf(":") + 1).toInt();

  // Ensure 2-digit format for hours and minutes
  char formattedTime[6];  // HH:MM format requires 5 chars + 1 for '\0'
  snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d", hour, minute);

  return String(formattedTime);  // Return the formatted string
}

void DeleteTime() {
  String time = server.arg("VAL");  // Get the time from the server
  time = formatTime(time);          // Format the time to HH:MM

  Serial.println("Deleting time: " + time);

  auto it = std::find(TIME_LIST.begin(), TIME_LIST.end(), time);
  if (it != TIME_LIST.end()) {
    TIME_LIST.erase(it);
    Serial.println("Element found and erased");
  } else {
    Serial.println("Element not found");
  }

  // Send updated list as JSON
  String jsonStr = "[";
  for (size_t i = 0; i < TIME_LIST.size(); i++) {
    jsonStr += "\"" + TIME_LIST[i] + "\"";
    if (i < TIME_LIST.size() - 1) {
      jsonStr += ",";
    }
  }
  jsonStr += "]";

  server.send(200, "application/json", jsonStr);
}

char* addElementToArray(std::vector<String>& timeList, const String& newElement) {
  timeList.push_back(newElement);

  // Convert vector to a JSON-like string (or you can choose another format)
  String jsonStr = "[";
  for (size_t i = 0; i < timeList.size(); i++) {
    jsonStr += "\"" + timeList[i] + "\"";
    if (i < timeList.size() - 1) {
      jsonStr += ",";
    }
  }
  jsonStr += "]";

  // Dynamically allocate memory for the updated string (note: we are using char* as an example)
  char* updatedArrayStr = new char[jsonStr.length() + 1];
  jsonStr.toCharArray(updatedArrayStr, jsonStr.length() + 1);

  return updatedArrayStr;
}

std::vector<String> traverseArray(const String& arrayStr) {
  std::vector<String> values;

  // Parse the string array manually, skipping the brackets
  int startIndex = 1;  // Skip the opening '['
  while (startIndex < arrayStr.length()) {
    int startQuote = arrayStr.indexOf("\"", startIndex);
    if (startQuote == -1) break;

    int endQuote = arrayStr.indexOf("\"", startQuote + 1);
    if (endQuote == -1) break;

    String element = arrayStr.substring(startQuote + 1, endQuote);
    values.push_back(element);

    startIndex = endQuote + 1;
  }

  return values;
}

bool timeExists(int h, int m) {
  // Loop through the list of times in TIME_LIST
  for (const auto& t : TIME_LIST) {
    // Extract the hour and minute from the string in the format HH:MM
    int hour = t.substring(0, t.indexOf(":")).toInt();
    int minute = t.substring(t.indexOf(":") + 1).toInt();

    // Compare the extracted hour and minute with the input parameters
    if (hour == h && minute == m) {
      return true;  // Time found
    }
  }
  return false;  // Time not found
}

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect(AP_SSID);

  Serial.println("Conectado a Wifi");
  Serial.println(WiFi.localIP());

  while (!MDNS.begin("purrfeeder")) {
    delay(1000);
  }

  server.on("/time", UpdateTime);
  server.on("/timedata", GetTimeList);
  server.on("/delete", DeleteTime);
  server.on("/", SendWebsite);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.begin();
}

void loop() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  Serial.print("Hora actual:");
  Serial.print(timeinfo.tm_hour-hour_offset);
  Serial.print(":");
  Serial.println(timeinfo.tm_min);

  if(timeExists(timeinfo.tm_hour-hour_offset, timeinfo.tm_min))
  {
    Serial.println("Hora de comer!");
  }

  server.handleClient();
  delay(100);
}
