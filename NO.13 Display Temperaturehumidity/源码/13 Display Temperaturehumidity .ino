#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <mySD.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "dd-wrt";
const char* password = "0000000000";

ESP32WebServer server(80);

float humidity = 0;
float temperature = 0;

const int led = 2;


void handleRoot() {
  /* we load the chart.html from microSD */
  File myFile = SD.open("CHRT~1.HTM");
  if (myFile) {  
    /* respond the content of file to client by calling streamFile()*/
    size_t sent = server.streamFile(myFile, "text/html");
    /* close the file */
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}
void handleLed1() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "1");
}
void handleLed0() {
  digitalWrite(led, 0);
  server.send(200, "text/html", "0");
}
void handleDHT(){
  /* read the data from dht22 sensor */
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  /* failed to read the data then return HTTP status code 503 sensor failed */
  if (isnan(humidity) || isnan(temperature)) {
    server.send(503, "text/html", "sensor failed");
  } else {
    /* we use Json "{"hum":#value, "temp": #value}" to respond the /dht22 request */
    root["hum"] = humidity;
    root["temp"] = temperature;
    char jsonChar[100];
    root.printTo((char*)jsonChar, root.measureLength() + 1);
    /* we respond Json style so content-type is text/json */
    server.send(200, "text/json", jsonChar);
  }
}
/* cannot handle request so return 404 */
void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  /* register the callbacks to process client request */
  /* root request we will read the memory card to get 
  the content of chrt.html and respond that content to client */
  server.on("/", handleRoot);
  /* this callback handle /dht22 request, 
  read the sensor form json string and respond*/
  server.on("/dht22", handleDHT);
  /* this callback handle /led1 request, just turn on led */
  server.on("/led1", handleLed1);
  /* this callback handle /led0 request, just turn off led */
  server.on("/led0", handleLed0);
  server.onNotFound(handleNotFound);
  server.begin();
  dht.begin();
  Serial.println("HTTP server started");
  Serial.print("Initializing SD card...");
  /* initialize microSD */
  if (!SD.begin(26, 14, 12, 27)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop(void){
  server.handleClient();
}