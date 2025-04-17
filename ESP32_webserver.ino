#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

const char *ssid = "ESP32_AP";  // Name of the Access Point
const char *password = "12341234";  // Password for the Access Point

WebServer server(80);
Preferences preferences;

void setupWiFiAP() {
  Serial.begin(115200);

  // Set up ESP32 as an Access Point
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);

  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  setupWiFiAP();
  setupServer();

  preferences.begin("settings", false);  // Open existing preferences or create if it doesn't exist

  // Get and print saved values
  String normalValue = preferences.getString("normal", "Default Normal");
  String lowValue = preferences.getString("low", "Default Low");
  String mediumValue = preferences.getString("medium", "Default Medium");
  String highValue = preferences.getString("high", "Default High");
  String genderValue = preferences.getString("gender", "Default Gender");

  Serial.println("Saved Values:");
  Serial.print("Normal: ");
  Serial.println(normalValue);
  Serial.print("Low: ");
  Serial.println(lowValue);
  Serial.print("Medium: ");
  Serial.println(mediumValue);
  Serial.print("High: ");
  Serial.println(highValue);
  Serial.print("Gender: ");
  Serial.println(genderValue);

  preferences.end();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String content = "<html><body style='text-align:center;'>";
  content += "<h1>Set Value</h1>";
  content += "<form id='myForm' method='post' action='/save'>";
  content += "<div style='margin:auto; width:50%;'>";

  // Add select tag for gender
  content += "<div style='text-align:left;'>Select Gender:</div>";
  content += "<select name='gender' id='gender' style='width: 150px; height: 30px; margin-left: 10px;'>";
  content += "<option value='male'>Male</option>";
  content += "<option value='female'>Female</option>";
  content += "</select><br>";

  // Add input fields for normal, low, medium, high
  content += "<div style='text-align:left;'>Enter Normal value:</div><input type='text' name='normal' id='normal' style='width: 100px; height: 30px; margin-left: 10px;'><br>";
  content += "<div style='text-align:left;'>Enter Low value:</div><input type='text' name='low' id='low' style='width: 100px; height: 30px; margin-left: 10px;'><br>";
  content += "<div style='text-align:left;'>Enter Medium value:</div><input type='text' name='medium' id='medium' style='width: 100px; height: 30px; margin-left: 10px;'><br>";
  content += "<div style='text-align:left;'>Enter High value:</div><input type='text' name='high' id='high' style='width: 100px; height: 30px; margin-left: 10px;'><br>";

  content += "<br><input type='submit' value='Save' style='background-color:green; color:white;'>";
  content += "</div></form></body></html>";

  server.send(200, "text/html", content);
}

void handleSave() {
  if (server.hasArg("normal") && server.hasArg("low") && server.hasArg("medium") && server.hasArg("high") && server.hasArg("gender")) {
    String normalValue = server.arg("normal");
    String lowValue = server.arg("low");
    String mediumValue = server.arg("medium");
    String highValue = server.arg("high");
    String genderValue = server.arg("gender");

    preferences.begin("settings", false);  // Open preferences
    preferences.putString("normal", normalValue);
    preferences.putString("low", lowValue);
    preferences.putString("medium", mediumValue);
    preferences.putString("high", highValue);
    preferences.putString("gender", genderValue);
    preferences.end();  // Close preferences

    Serial.println("Values saved successfully.");
    server.send(200, "text/html", "Values saved successfully.");
  } else {
    Serial.println("Error: Values not provided.");
    server.send(400, "text/plain", "Error: Values not provided.");
  }
}
