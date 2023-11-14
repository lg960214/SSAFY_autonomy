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
bool is_smartthings_action = false;
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

    ESP_LOGW(TAG, "Start Loop");
    while(true) {
        xLastWakeTime = xTaskGetTickCount();
        timer = time(NULL);
        localtime_r(&timer, &g_time);

        // 00:00시가 되면
        if(g_time.tm_min ==0  && g_time.tm_sec== 0) g_model_update = true;
        
        if(g_model_update) {
            get_tflite_file();
            setModel();
            led_after_model_update();
            g_AI_mode = true;
            g_model_update = false;
            is_smartthings_action = false;
        }

        // get sensor data
        g_motion = get_motion_detect_state();
        g_illuminance = get_illumi_value();
        g_light_brightness = light_get_brightness();
        
        if(g_AI_mode) {
            g_light_brightness = inference(1023-g_illuminance, g_AI_mode, g_light_brightness, g_motion, g_time);
            ESP_LOGW(TAG, "brightness : %d", g_light_brightness);
            if(g_light_brightness <= 30 )
                light_set_brightness(0);
            else
                light_set_brightness(g_light_brightness);
        }

        send_sensor_data(g_illuminance, g_AI_mode, g_light_brightness, g_time);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
