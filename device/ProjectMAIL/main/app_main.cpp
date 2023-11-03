#include <esp_log.h>

static const char* TAG = "app_main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello");
}
