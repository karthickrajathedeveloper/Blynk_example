//Code by: M.Karthickraja
//ESP32 - Receive the API Value
/*[
  {
    "id": 1,
    "Column 1": "Tory Spata"
  }
]
 *API receive array json format
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> 

const char* ssid = "Petal";
const char* password = "Petal#2021";

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
WiFi.begin(ssid, password);
while(WiFi.status() != WL_CONNECTED){
  delay(1000);
  Serial.println("Connecting to WiFi...");
}
Serial.println("Connected to WiFi");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() ==WL_CONNECTED)
  {
    HTTPClient http;
    //set the API URL
    http.begin("https://retoolapi.dev/Gpi1wW/data");
    //Make the API request
    int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      //Serial.println(request); //Print the received data
      DynamicJsonDocument doc(1024); //Adjust the buffer size 
      DeserializationError error = deserializeJson(doc, payload);
      if(error){
        Serial.println("Failed to parse json");
      }
      else{
        //Extract the Json values
        JsonArray jsonArray = doc.as<JsonArray>();
          for (JsonObject obj : jsonArray) {
          int id = obj["id"];
          const char* column = obj["Column 1"];
          Serial.println(id);
          Serial.println(column);
        }
      }
    }
    else{
      //Error in the request
      Serial.print("HTTP request failed: ");
      Serial.println(httpCode);
    }
    http.end();
  }
  delay(2000);
}
