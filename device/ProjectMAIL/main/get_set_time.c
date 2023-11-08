#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "http_client.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_tls.h"
#include "protocol_examples_common.h"

static const char* TAG = "GET_SET_TIME";
#define MAX_HTTP_OUTPUT_BUFFER 100

void set_time(void)
{
    // wifi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Connected to AP, begin http example");

    // FAST Sever time 동기화
    int server_sec = 0;
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    esp_http_client_config_t config = {
        .url = "http://43.201.16.59:8001/MaiL/gettime",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    server_sec = atoi(local_response_buffer);

    // 시간 설정
    struct timeval initial_timeval = {
        .tv_sec = server_sec,
        .tv_usec = 0
    };

    int error = settimeofday(&initial_timeval, NULL);
    assert(error == 0);
    return ;
}

void get_time(void)
{
    // Get current time as 'struct timeval'.
    // See https://linux.die.net/man/2/gettimeofday
    struct timeval new_timeval;
    int err = gettimeofday(&new_timeval, NULL);
    assert(err == 0);
    // Extract Unix time
    time_t new_unix_time = new_timeval.tv_sec;
    // Convert to broken-down time
    // See https://en.cppreference.com/w/c/chrono/localtime
    struct tm new_time;
    localtime_r(&new_unix_time, &new_time);
    // 'new_time' now contains the current time components
    printf("Current time: %s", asctime(&new_time));
}