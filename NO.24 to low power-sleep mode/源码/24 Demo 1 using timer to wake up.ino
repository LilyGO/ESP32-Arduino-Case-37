#include <Preferences.h>

/* possible states */
typedef enum {
  SETUP = 1,
  RUNNING  
}State;

State state = SETUP;
Preferences preferences;

void setup() {
  Serial.begin(115200);
  Serial.println("I waked up");
  /* setup Preferences */
  preferences.begin("iotsharing", false);
  /* restoring state, if not exist return default SETUP sate */
  state = (State)preferences.getUInt("state", SETUP);
}
void loop() {
  switch(state){
    case SETUP:
      Serial.println("SETUP state");
      state = RUNNING;
      break;
    case RUNNING:
      Serial.println("RUNNING state");
      /* save current state */
      preferences.putUInt("state", state);
      /* Close the Preferences */
      preferences.end();
      Serial.println("Bring ESP to sleep mode 5 seconds");
      /* 5e6 is 5x10^6 microseconds */
      ESP.deepSleep(5e6);
      break;
  }
}