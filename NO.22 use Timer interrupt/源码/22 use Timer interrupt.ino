/* create a hardware timer */
hw_timer_t * timer = NULL;

/* LED pin */
int led = 14;
/* LED state */
volatile byte state = LOW;

void IRAM_ATTR onTimer(){
  state = !state;
  digitalWrite(led, state);
}

void setup() {
  Serial.begin(115200);

  pinMode(led, OUTPUT);

  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);

  /* Attach onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer, true);

  /* Set alarm to call onTimer function every second 1 tick is 1us
  => 1 second is 1000000us */
  /* Repeat the alarm (third parameter) */
  timerAlarmWrite(timer, 1000000, true);

  /* Start an alarm */
  timerAlarmEnable(timer);
  Serial.println("start timer");
}

void loop() {

}