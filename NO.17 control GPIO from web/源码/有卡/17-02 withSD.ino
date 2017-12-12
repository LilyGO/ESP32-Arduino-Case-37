#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <mySD.h>

const char* ssid = "dd-wrt";
const char* password = "0000000000";

ESP32WebServer server(80);

void handleRoot() {
  /* we load the chart.html from microSD */
  File myFile = SD.open("INDEX~1.HTM");
  if (myFile) {  
    /* respond the content of file to client by calling streamFile()*/
    size_t sent = server.streamFile(myFile, "text/html");
    /* close the file */
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}
/* this callback is to handle user request
req?out=0o1 -> user request GPIO0 output value HIGH 
req?in=0i -> user request GPIO0 input */
void handleGPIO() {
  
  if(server.args() > 0){
    String req = server.argName(0);
    if(req == "out"){
      server.send(200, "text/plain", "OK");
      /* this code is to parse req */
      String param = server.arg(0);
      int p = param.indexOf('o');
      int pin = param.substring(0,p).toInt();
      int value = param.substring(p+1).toInt();
      /* set pin and output value */
      pinMode(pin, OUTPUT);
      digitalWrite(pin, value);
      
    }else if(req == "in"){
      String param = server.arg(0);
      /* this code is to parse req */
      int p = param.indexOf('i');
      int pin = param.substring(0,p).toInt();
      /*set pin and read input */
      pinMode(pin, INPUT);
      int value = digitalRead(pin);
      char str[3] = {0,0,0};
      sprintf(str, "%d", value);
      /* respond the read value */
      server.send(200, "text/plain", str);
    }
  }
}
/* cannot handle request so return 404 */
void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void){
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
  /* this callback handle GPIO request and respond*/
  server.on("/req", handleGPIO);

  server.onNotFound(handleNotFound);
  server.begin();

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