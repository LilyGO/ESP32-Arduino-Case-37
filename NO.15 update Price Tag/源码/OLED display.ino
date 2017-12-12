#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SH1106.h>

#define OLED_SDA 12
#define OLED_SCL 14

Adafruit_SH1106 display(12, 14);
/* change it with your ssid-password */
const char* ssid = "dd-wrt";
const char* password = "0000000000";
/* this is the IP of PC where you installed MQTT Server */
const char* mqtt_server = "192.168.1.103";

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/* topics */
#define IP7_PRICE_TOPIC    "update/pricetag/ip7"

long lastMsg = 0;
char msg[20];
#define BUF_SIZE 100
byte rec[BUF_SIZE];
int separate = 0;
int dispState = 0;
  
void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  int i = 0;
  Serial.print("payload: ");
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    /* we parse topic by separator '-'*/
    if(payload[i] == '-'){
      separate = i+1;
      payload[i] = 0;
    }
    if(i < BUF_SIZE){
      rec[i] = payload[i];
    }
  }
  /*we got data so backup for processing 
  if we have enough buffer size, else ignore msg */
  if(i < BUF_SIZE){
    if(dispState == 0){
      dispState = 1;
    }
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic with QoS 1*/
      client.subscribe(IP7_PRICE_TOPIC, 1);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  /* while flashing sw detach OLED VCC w ESP32 3.3,
  delay here to have enough time to attach 
  OLED VCC with ESP32 3.3 again */
  delay(3000);
  /* initialize oled */
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  /* set text size, color, cursor position, 
  set buffer with  Hello world and show off*/
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(2,2);
  display.println("iotsharing.com");
  display.display();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
}
void loop() {
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming 
  subscribed topic-process-invoke receivedCallback */
  client.loop();
  /* because OLED waste time 
  so we use state machine to display 
  step by step every loop */
  switch(dispState)
  {
    case 1:
    display.clearDisplay();
    dispState = 2;
    break;
    case 2:
    display.setCursor(2,2);
    dispState = 3;
    break;
    case 3:
    display.println((char *)rec);
    dispState = 4;
    break;
    case 4:
    display.setCursor(2,22);
    dispState = 5;
    break;
    case 5:
    display.println((char *)&rec[separate]);
    dispState = 6;
    break;
    case 6:
    display.display();
    dispState = 0;
    memset(rec, 0, BUF_SIZE);
    break;
    default:
    break;
  }
}