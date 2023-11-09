#include <esp_log.h>
//#include <esp_err.h>

#include "light_control.h"
#include "sensor.h"
#include "matter_start.h"
#include "ondevice_ai.h"
#include "matter_start.h"
#include "ondevice_ai.h"

static const char* TAG = "app_main";

bool g_AI_mode = false;
bool g_model_update = false;
SensorData g_sensor_data;
uint8_t g_light_brightness;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");

    // setting
    matter_start();
    setModel();

    // main process
    get_sensor_data(&g_sensor_data);
    if(g_AI_model) {
        g_light_brightness = inference();
        light_set_brightness(g_light_brightness);
    }

    // TODO: Call data POST
    
    if(g_model_update) {
        // @ enduser0
        // TODO: Call update GET
    }
}