#include <esp_log.h>
//#include <esp_err.h>

#include "light_control.h"
#include "sensor.h"
#include "matter_start.h"
#include "ondevice_ai.h"
#include "matter_start.h"
#include "ondevice_ai.h"

// FAST server REST API
extern "C" {
    #include "http_client.h"
    #include "my_time.h"
    #include <unistd.h>
}

static const char* TAG = "app_main";

bool g_AI_mode = false;
bool g_model_update = true;
Sensor g_sensor_data;
uint8_t g_light_brightness;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");

    // setting
    matter_start();
    sensor_init();
    //setModel();
    set_time();

    // main process
    get_sensor_data(&g_sensor_data);
    if(g_AI_mode) {
        g_light_brightness = inference();
        light_set_brightness(g_light_brightness);
    }

    // TODO: Call data POST
    /
    int g_illuminance = 0;
    int g_auto = 0;
    int g_on = 1;
    int g_brightness = 1000;
    send_sensor_data(g_illuminance, g_auto, g_on, g_brightness);
    
    // TODO: Call update GET
    if(g_model_update) {
		get_tflite_file();
		setModel();
    }

	g_light_brightness = inference();
	ESP_LOGI(TAG, "bright: %d", g_light_brightness);
}
