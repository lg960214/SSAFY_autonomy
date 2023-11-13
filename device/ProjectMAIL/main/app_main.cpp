#include <esp_log.h>
//#include <esp_err.h>

#include "light_control.h"
#include "sensor.h"
#include "matter_start.h"
#include "ondevice_ai.h"

// FAST server REST API
extern "C" {
    #include "http_client.h"
    #include "get_set_time.h"
    #include <unistd.h>
}

static const char* TAG = "app_main";

bool g_AI_mode = true; // AI 모드인가?
bool g_model_update = true; // model update가 필요한가?
time_t timer; // device time
struct tm g_time; // device time 유지용
int g_motion; // 움직임 value
uint32_t g_illuminance; // 조도 value
uint8_t g_light_brightness; // 결과값 (조명 밝기)

// SensorData g_sensor_data;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");

// setting
    matter_start();
    sensor_init();
    //setModel();
    set_time();

    // Main process
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    while(true) {

        xLastWakeTime = xTaskGetTickCount();
        timer = time(NULL);
        localtime_r(&timer, &g_time);
        ESP_LOGI(TAG, "mon: %d, sec: %d, wday: %d, hour: %d", g_time.tm_mon, g_time.tm_sec, g_time.tm_wday, g_time.tm_hour);

        if(g_model_update) {
            get_tflite_file();
            setModel();
            g_model_update = false;
        }

        // get sensor data
        g_motion = get_motion_detect_state();
        g_illuminance = get_illumi_value();
        g_light_brightness = light_get_brightness();
        
        if(g_AI_mode) {
            g_light_brightness = inference(g_illuminance, g_AI_mode, g_light_brightness, g_motion, g_time);
            light_set_brightness(g_light_brightness);
        }

        send_sensor_data(g_illuminance, g_AI_mode, g_light_brightness, g_time);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
