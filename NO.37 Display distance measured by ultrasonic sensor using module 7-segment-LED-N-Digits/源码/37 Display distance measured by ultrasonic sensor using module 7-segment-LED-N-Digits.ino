#include "EspLed7SegNDigit.h"

//LED pins
int SCLK = 14; //pulse
int RCLK = 27; //latch
int DIO = 12;  //data

//utrasonic pins
int TRIG_PIN = 25;
int ECHO_PIN = 33;

unsigned long startMeasure = 0;
unsigned long endMeasure = 0;
unsigned long measureTime = 0;
unsigned long distance = 0;

EspLed7SegNDigit ledm(SCLK, RCLK, DIO, 8);

void updateDisplayCb(void){
  ledm.clearDisplay();
  ledm.setCharAt(8, 'd');
  ledm.setCharAt(7, 's');
  ledm.setCharAt(6, 't');
  ledm.displayNum(distance, 3);
}

void initUltra(){
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void ultraTask(){

  //trigger sensor with pulse LOW-HIGH-LOW
  digitalWrite(TRIG_PIN, LOW);
  //wait 2 us
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  //wait 10 us
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  //at the beginning ECHO pin will be pull LOW until finishing transmitting ultrasonic signal
  while (digitalRead(ECHO_PIN) == 0){
    startMeasure = micros();
  }
  //ECHO pin will be pulled HIGH until get response
  while (digitalRead(ECHO_PIN) == 1){
    endMeasure = micros();
  }
  
  //response time will be calculated by
  measureTime = endMeasure - startMeasure;

  //convert to cm
  distance = (measureTime)/29/2;
  Serial.printf("distance = %d\n", distance);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initUltra();
  //this callback will be invoked for updating new ultrasonic value
  ledm.setUpdateCb(updateDisplayCb);

  ledm.setCharAt(8, 'd');
  ledm.setCharAt(7, 's');
  ledm.setCharAt(6, 't');
  //in loop it take time to do ultraTask so we decrease refresh time
  ledm.setRefreshTime(100);
}
long tick = 0;
void loop() {
  // put your main code here, to run repeatedly:
  ledm.loop();
  long now = millis();
  if (now - tick > 1000) {
    tick = now;
    ultraTask();
  }
}