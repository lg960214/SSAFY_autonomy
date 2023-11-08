#include <esp_log.h>
//#include <esp_err.h>

#include "light_control.h"
#include "sensor.h"
#include "matter_start.h"
#include "ondevice_ai.h"

static const char* TAG = "app_main";
// static esp_adc_cal_characteristics_t adc1_chars;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");

    matter_start();

    setModel();

    inference();
    
}

