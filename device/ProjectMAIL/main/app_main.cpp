#include <esp_log.h>
//#include <esp_err.h>

// #include "ondevice_ai.h"

// FAST server REST API
static const char* TAG = "app_main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");
    my_printf();
    
    // 조도센서 측정값
    int g_illuminance = 1023;
    // 수동 여부
    int g_auto = 1;
    // led on 여부
    int g_on = 0;
    // led 밝기
    int g_brightness = 1022;
    
    // 센서 데이터를 1초에 한번씩 보내는 함수
    send_sensor_data(g_illuminance, g_auto, g_on, g_brightness);
}