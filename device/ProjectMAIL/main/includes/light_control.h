
#include <esp_log.h>
#include <esp_err.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <esp_matter.h>
#include <led_driver.h>

#include <app_priv.h>

esp_err_t light_set_power(bool power);
esp_err_t light_set_brightness(uint8_t val);
bool light_get_power();
int8_t light_get_brightness();
void led_after_model_update();
