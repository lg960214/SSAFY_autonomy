#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "http_client.h"
#include "esp_log.h"

#include "get_set_time.h"

#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_tls.h"
#include "protocol_examples_common.h"

static const char* TAG = "GET_SET_TIME";
#define MAX_HTTP_OUTPUT_BUFFER 100

void set_time(void)
{

    // FAST Sever time 동기화
    int server_sec = 0;
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    esp_http_client_config_t config = {
        .url = "http://13.125.12.50:8002/MaiL/gettime",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET
    esp_err_t err = esp_http_client_perform(client);
    while (err != ESP_OK) {
        err = esp_http_client_perform(client);
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    
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