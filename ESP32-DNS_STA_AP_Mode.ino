#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

// Access Point credentials
const char* ap_ssid = "ESP32-AP";
const char* ap_password = "12345678";

String received_ssid = "";
String received_password = "";

void setup() {
  Serial.begin(115200);
  delay(100);

  // Start AP mode
  Serial.println("Starting AP...");
  WiFi.softAP(ap_ssid, ap_password);
  delay(100);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  // Start mDNS (won't work in AP mode but kept as per your code)
  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) delay(1000);
  }
  Serial.println("mDNS responder started");

  // Routes
  server.on("/", handle_OnConnect);
  server.on("/submit", handle_FormSubmit);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// HTML page with form
void handle_OnConnect() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 WiFi Setup</title>
    </head>
    <body>
      <h2>Enter WiFi Credentials</h2>
      <form action="/submit" method="GET">
        SSID:<br><input type="text" name="ssid"><br>
        Password:<br><input type="password" name="pass"><br><br>
        <input type="submit" value="Submit">
      </form>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Handle form submission
void handle_FormSubmit() {
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    received_ssid = server.arg("ssid");
    received_password = server.arg("pass");

    Serial.println("Received SSID: " + received_ssid);
    Serial.println("Received Password: " + received_password);

    server.send(200, "text/html", "<h3>Received credentials:</h3><br>SSID: " + received_ssid + "<br>Password: " + received_password);
    
    // Optionally: save to preferences or try to connect to STA here
  } else {
    server.send(400, "text/html", "Missing SSID or password!");
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Page not found");
}
