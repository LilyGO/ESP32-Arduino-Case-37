#include "esp_deep_sleep.h"

void setup() {
  Serial.begin(115200);
  Serial.println("I waked up");
  /* set up RTC_IO for GPIO12 */
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
  /* enable pullup resister */
  gpio_pullup_en(GPIO_NUM_12);
  /* disable pullup resister */
  gpio_pulldown_dis(GPIO_NUM_12); // not use pulldown on GPIO
  /* anable deep sleep with external wakeup
  and input level which will trigger wakeup is LOW on GPIO12 */
  esp_deep_sleep_enable_ext0_wakeup(GPIO_NUM_12, LOW);

  Serial.println("Bring ESP to sleep mode until get external signal");
  esp_deep_sleep_start();
}

void loop() {
}