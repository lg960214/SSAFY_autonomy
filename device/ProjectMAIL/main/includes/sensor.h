#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define MOTION_SENSOR_GPIO GPIO_NUM_14
#define IllUMINANCE_SENSOR_GPIO GPIO_NUM_27

typedef struct SensorData {
    int motion_state;
    uint32_t illumi_value;
};

int get_motion_detect_state();
uint32_t get_illumi_value();
void sensor_init();
void get_sensor_data(SensorData*);