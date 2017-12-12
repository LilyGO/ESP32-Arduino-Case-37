#include <WiFiClient.h>
#include "ESP32WebServer.h"
#include <WiFi.h>
#include <ESPmDNS.h>

/* change your ssid and password here */
const char* ssid = "TP-LINK_888";
const char* password = "Zhang520521";

ESP32WebServer server(80);

const int servoPin = 13;
int oldAngle = 0; 
int PWM_WIDTH = 20000;

/* this array contains the web will be responded to client
it uses jquery for making GET request and processing slider UI control */
char res[900]=
"<!DOCTYPE html>\
<html>\
<head>\
<meta charset='utf-8'>\
<H1>iotsharing.com Servo</H1>\
<link href='https://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css' rel='stylesheet'>\
<script src='https://code.jquery.com/jquery-1.10.2.js'></script>\
<script src='https://code.jquery.com/ui/1.10.4/jquery-ui.js'></script>\
<script>\
$(function() {\
$('#sliVal').html('Angle: 0');\
$('#slider').slider({\
    orientation:'vertical',value:0,min: 0,max: 180,step: 5\
});\
$('#slider').slider().bind({\
slidestop: function(e,ui){\
    $('#res').css('background','red');\
    $('#sliVal').html('Angle: '+ui.value);\
    $.get('/ang?val=' + ui.value, function(d, s){\
        $('#res').css('background','green');\
        $('#res').html(s);\
    }); \
}\
});\
});\
</script>\
</head>\
<body>\
<div id='slider'></div></br>\
<div id='sliVal'></div>\
<div id='res'></div>\
</body>\
</html>";

void handleRoot() {
  server.send(200, "text/html", res);
}
/* this function map from angle to pulse width */
int servoPulse(int angleDegrees)
{
  int pulseWidth = map(angleDegrees, 0,180,600,2400);
  return pulseWidth;
}
/* this function check the rotation angle
and trigger pulse accordingly*/
void servoGo(int oldAngle, int newAngle)
{
  int pulseWidth;
  if(oldAngle == newAngle){
    return;
  }else if(oldAngle < newAngle){
    /* clockwise processing */
    for (int i=oldAngle; i<=newAngle; i++){
      /* convert angle to pulse width us*/
      pulseWidth = servoPulse(i);
      /* trigger HIGH pulse */  
      digitalWrite(servoPin, HIGH);
      /* use delayMicroseconds to delay for pulseWidth */
      delayMicroseconds(pulseWidth); 
      /* trigger LOW pulse */                          
      digitalWrite(servoPin, LOW);
      /* use delayMicroseconds to delay 
      for rest time (20000 - pulseWidth) */ 
      delayMicroseconds(PWM_WIDTH - pulseWidth);   
    }
  }else if(oldAngle > newAngle){
    /* anti-clockwise processing */
    for (int i=oldAngle; i>=newAngle; i--){
      pulseWidth = servoPulse(i);
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(pulseWidth);                           
      digitalWrite(servoPin, LOW);
      delayMicroseconds(PWM_WIDTH - pulseWidth);
    }
  }
}
/* this callback will be invoked when get servo rotation request */
void handleServo() {
  //Serial.println(server.argName(0));
  int newAngle = server.arg(0).toInt();
  servoGo(oldAngle, newAngle);
  oldAngle = newAngle;
  server.send(200, "text/html", "ok");
}

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
  
  server.on("/", handleRoot);
  server.on("/ang", handleServo);

  server.onNotFound(handleNotFound);

  server.begin();
  pinMode(servoPin, OUTPUT);
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
