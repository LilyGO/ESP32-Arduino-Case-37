#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>

/*fill your ssid and password here */
const char* ssid = "mieu mieu 01";
const char* password = "09471919479";

ESP32WebServer server(80);

/* this callback will be invoked when user request "/" */
void handleRoot() {
  /* server respond 200 with content "hello from ESP32!" */
  server.send(200, "text/plain", "hello from ESP32!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  /* Wait for connection */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /* we use mDNS here http://esp32.local */
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  /* register callback function when user request root "/" */
  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);
  /* start web server */
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}