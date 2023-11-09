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
    int illumi_value = adc1_get_raw(ADC1_CHANNEL_6); 
    return illumi_value;
}

void get_sensor_data(Sensor *s){
    s->motion_state = get_motion_detect_state();
    s->illumi_value = get_illumi_value();
}
