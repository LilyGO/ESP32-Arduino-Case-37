#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>

/* thay doi ssid va password */
const char* ssid = "dd-wrt";
const char* password = "0000000000";

ESP32WebServer server(80);

/* day la phan code giao dien, dung jquery va boostrap */
char res[3000] = 
"<!DOCTYPE html>\n\
<html>\n\
<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=1'>\n\
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>\n\
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>\n\
<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>\n\
</head>\n\
<body>\n\
\n\
<div class='container'>\n\
<h2>iotsharing.com - ESP32 WEBGPIO</h2>\n\
</br>  \n\
<table class='table'>\n\
<thead>\n\
<tr>\n\
<th>Status</th>\n\
<th>GPIO</th>\n\
<th>In/Out</th>\n\
<th>Write HIGH</th>\n\
<th>Write LOW</th>\n\
<th>Read</th>\n\
</tr>\n\
</thead>\n\
<tbody>\n\
<tr>\n\
<td>  \n\
<button class='btn btn-default dropdown-toggle' type='status' id='status' >#status</button>\n\
</td>\n\
<td> \n\
<select id = 'gpio' class='btn btn-default dropdown-toggle'>\n\
<option>GPIO0</option>\n\
<option>GPIO2</option>\n\
<option>GPIO4</option>\n\
</select>\n\
</td>\n\
<td>  \n\
<select id = 'dir' class='btn btn-default dropdown-toggle'>\n\
<option>Out</option>\n\
<option>In</option>\n\
</select>\n\
</td>\n\
<td>  \n\
<button class='btn btn-default dropdown-toggle' type='button' id='write1' >Write HIGH</button>\n\
</td>\n\
<td>  \n\
<button class='btn btn-default dropdown-toggle' type='button' id='write0' >Write LOW</button>\n\
</td>\n\
<td>  \n\
<button class='btn btn-default dropdown-toggle' type='button' id='read' >Read</button>\n\
</td>\n\
</tr>\n\
</tbody>\n\
</table>                                        \n\
</div>\n\
<script>\n\
function ml(gpio, dir, value) {\n\
var io = gpio.replace('GPIO', '');\n\
var d = (dir == 'In') ? 'i' : 'o';\n\
if(value != null){\n\
var v = (value == 'Write LOW') ? '0' : '1';\n\
return (io+d+v);    \n\
} else {\n\
return (io+d);  \n\
}\n\
}\n\
$(function() {\n\
$('#status').css('background','grey');\n\
$('#write1').on('click', function (e) {\n\
var dir = $('#dir').val();\n\
$('#status').css('background','red');\n\
$('#status').html('#status');\n\
if(dir == 'In'){\n\
alert('direction is In');\n\
} else {\n\
var gpio = $('#gpio').val();\n\
var value = $('#write1').text();\n\
$.get('/req?out=' + ml(gpio, dir, value), function(d, s){\n\
$('#status').css('background','green');\n\
$('#status').html(s);\n\
});      \n\
}\n\
})\n\
$('#write0').on('click', function (e) {\n\
var dir = $('#dir').val();\n\
$('#status').css('background','red');\n\
$('#status').html('#status');\n\
if(dir == 'In'){\n\
alert('direction is In');\n\
} else {\n\
var gpio = $('#gpio').val();\n\
var value = $('#write0').text();\n\
$.get('/req?out=' + ml(gpio, dir, value), function(d, s){\n\
$('#status').css('background','green');\n\
$('#status').html(s);\n\
});        \n\
}\n\
})\n\
$('#read').on('click', function (e) {\n\
var dir = $('#dir').val();\n\
$('#status').css('background','red');\n\
$('#status').html('#value');\n\
if(dir == 'Out'){\n\
alert('direction is Out');\n\
} else {\n\
var gpio = $('#gpio').val();\n\
$.get('/req?in=' + ml(gpio, dir, null), function(d, s){\n\
if(s == 'success'){\n\
$('#status').css('background','yellow');\n\
$('#status').html(d);\n\
}else{\n\
$('#status').html(s);\n\
}               \n\
});     \n\
}\n\
})\n\
});\n\
</script>\n\
</body>\n\
</html>";

void handleRoot() {
    server.send(200, "text/html", res);
}
/* ham nay de xu lu yeu cau cua nguoi dung
req?out=0o1 -> user request GPIO0 output gia tri HIGH 
req?in=0i -> user request GPIO0 input */
void handleGPIO() {
  Serial.println("got request");
  if(server.args() > 0){
    String req = server.argName(0);
    if(req == "out"){
      server.send(200, "text/plain", "OK");
      /* parse req */
      String param = server.arg(0);
      int p = param.indexOf('o');
      int pin = param.substring(0,p).toInt();
      int value = param.substring(p+1).toInt();
      /* cai dat pin va ghi gia tri */
      pinMode(pin, OUTPUT);
      digitalWrite(pin, value);
    }else if(req == "in"){
      String param = server.arg(0);
      /* parse req */
      int p = param.indexOf('i');
      int pin = param.substring(0,p).toInt();
      /* cai dat pin va doc input */
      pinMode(pin, INPUT);
      int value = digitalRead(pin);
      char str[3] = {0,0,0};
      sprintf(str, "%d", value);
      /* respond cho nguoi dung */
      server.send(200, "text/plain", str);
    }
  }
}
/* cannot handle request so return 404 */
void handleNotFound(){
  String message = "File Not Found\n\n\n\n";
  server.send(404, "text/plain", message);
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  /*cho wifi ket not */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /* nhung ham nay se duoc goi khi co user request va respond */
  server.on("/", handleRoot);
  /* this callback handle GPIO request and respond*/
  server.on("/req", handleGPIO);

  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}