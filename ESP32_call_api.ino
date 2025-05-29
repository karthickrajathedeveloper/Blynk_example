#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Server URL (Flask server running)
const char* serverName = "http://your ip:port/route";  // Change IP to your server

// UID value (dummy for now)
String uidValue = "12345678";

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Call the API when UID is available (replace with your RFID read function)
  apiCall();

  // Wait for 5 seconds before next call
  delay(5000);
}

void apiCall() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    // Prepare JSON payload
    DynamicJsonDocument doc(512);
    doc["uid"] = uidValue;
    String requestBody;
    serializeJson(doc, requestBody);

    // Send POST Request
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(requestBody);

    // Check response
    if (httpResponseCode > 0) {
      String responseBody = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(responseBody);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
