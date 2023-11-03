#include <esp_log.h>
//#include <esp_err.h>

#include "ondevice_ai.h"

static const char* TAG = "app_main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");
    
    printTest();
}
