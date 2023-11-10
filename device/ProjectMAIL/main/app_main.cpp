#include <esp_log.h>
//#include <esp_err.h>

#include "light_control.h"
#include "sensor.h"
#include "matter_start.h"
#include "ondevice_ai.h"

// FAST server REST API
extern "C" {
    #include "http_client.h"
    #include "my_time.h"
    #include <unistd.h>
}

static const char* TAG = "app_main";

bool g_AI_mode = true;
bool g_model_update = false;
uint8_t g_light_brightness;
SensorData g_sensor_data;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");

    // setting
    // matter_start();
    setModel();
    set_time();

    inference();

    // TODO: Call data POST
    get_ftlite_file();
    
    if(g_model_update) {
        // @ enduser0
        // TODO: Call update GET
        send_sensor_data(g_illuminance, g_auto, g_on, g_brightness);
    }
}