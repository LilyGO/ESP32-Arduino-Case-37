#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/* change it with your ssid-password */
const char* ssid = "dd-wrt";
const char* password = "0000000000";
/* this is the IP of PC where you installed MQTT Server */
const char* mqtt_server = "192.168.1.103";

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);
/* this is for LCD */
LiquidCrystal_I2C lcd(0x27,16,2);

/* topics */
#define SSS8_PRICE_TOPIC    "update/pricetag/sss8"

long lastMsg = 0;
char msg[20];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);
  int separate = 0;
  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    /* we parse topic by separator '-'*/
    if(payload[i] == '-'){
      separate = i+1;
      payload[i] = 0;
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print((char *)payload);
  /* set cursor row 1 col 0 */
  lcd.setCursor(0,1);
  lcd.print((char *)&payload[separate]);
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
      /* subscribe topic */
      client.subscribe(SSS8_PRICE_TOPIC);
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
  /* initialize the lcd with SDA and SCL pins */
  lcd.begin(12, 14);
  /* Print a message to the LCD.*/
  lcd.backlight();
  /* set cursor row 0 col 0 */
  lcd.setCursor(0,0);
  lcd.print("iotsharing.com");
  /* connect wifi */
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
}