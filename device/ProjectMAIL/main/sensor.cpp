#include "sensor.h"


void sensor_init(){
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // channel6 is gpio 34
}
//움직임 감지 센서
int get_motion_detect_state(){
    gpio_set_direction(MOTION_SENSOR_GPIO, GPIO_MODE_INPUT);
    int state = gpio_get_level(MOTION_SENSOR_GPIO);
    return state;
}

//조도 센서 config
uint32_t get_illumi_value(){
    // adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_0);
    int reading=0;
    adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_BIT_9, &reading);
    // uint32_t reading =  adc1_get_raw(ADC1_CHANNEL_5);
    // uint32_t voltage = esp_adc_cal_raw_to_voltage(reading, adc_chars);
    // gpio_set_direction(IllUMINANCE_SENSOR_GPIO, GPIO_MODE_INPUT);
    return reading;
}