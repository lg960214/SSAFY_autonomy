#include <esp_log.h>
//#include <esp_err.h>

// #include "ondevice_ai.h"
#include "matter_start.h"
#include "light_control.h"
#include "sensor.h"

// FAST server REST API
static const char* TAG = "app_main";
static esp_adc_cal_characteristics_t adc1_chars;

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
    
    matter_start();
    
    // esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11)); // channel6 is gpio 34
    while(true){
        // light_set_power(true);
        // light_set_brightness(200); //0~255
        // sleep(5);
        // light_set_power(false);
        // light_set_brightness(50);
        // sleep(5);
        if(get_motion_detect_state()==0){
            ESP_LOGW(TAG, "No detect");
        }else
            ESP_LOGI(TAG, "detected!");
        int ill = adc1_get_raw(ADC1_CHANNEL_6); 
        ESP_LOGI(TAG, "illumi value is %d", ill);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    // printTest();
}
//CONFIG_FREERTOS_WATCHPOINT_END_OF_STACK=y
