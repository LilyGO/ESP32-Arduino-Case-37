#include <Preferences.h>

/* create an instance of Preferences library */
Preferences preferences;

void setup() {
  Serial.begin(115200);
  Serial.println();

  /* Start a namespace "iotsharing"
  in Read-Write mode: set second parameter to false 
  Note: Namespace name is limited to 15 chars */
  preferences.begin("iotsharing", false);

  /* if you want to remove all preferences under opened namespace uncomment it */
  //preferences.clear();

  /* if we want to remove the reset_times key uncomment it */
  //preferences.remove("reset_times");

  /* get value of key "reset_times", if key not exist return default value 0 in second argument
  Note: Key name is limited to 15 chars too */
  unsigned int reset_times = preferences.getUInt("reset_times", 0);

  /* we have just reset ESP then increase reset_times */
  reset_times++;

  Serial.printf("Number of restart times: %d\n", reset_times);

  /* Store reset_times to the Preferences */
  preferences.putUInt("reset_times", reset_times);

  /* Close the Preferences */
  preferences.end();
}

void loop() 
{
}