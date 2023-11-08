#include <esp_spiffs.h>
#include <algorithm>
#include <esp_log.h>
#include <cstring>
#include <ctime>
#include <cstdio>
#include "esp_http_client.h"

#include "esp_tls.h"
#include <sys/param.h>

#include "protocol_examples_common.h"

#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls.h"
#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


using namespace std;

#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "HTTP_CLIENT";

void test_file_system()
{
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello World!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/test.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = min(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = min(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED : {
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        }
        case HTTP_EVENT_REDIRECT: {
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
        }
    }
    return ESP_OK;
}

// [년],[월],[일],[시],[분],[초],[요일],[조도센서 측정값],[수동모드 여부],[LED 의 밝기],[LED 전원 On 여부]
// 2023,11,02,12,00,00,2,1023,1,1023,1
void send_sensor_data(int illuminance, int is_auto, int is_on, int brightness)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    //ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Connected to AP, begin http example");

    ESP_LOGI(TAG, "Enter send_sensor_data");
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    /**
     * NOTE: All the configuration parameters for http_client must be spefied either in URL or as host and path parameters.
     * If host and path parameters are not set, query parameter will be ignored. In such cases,
     * query parameter should be specified in URL.
     *
     * If URL as well as host and path parameters are specified, values of host and path will be considered.
     */
    esp_http_client_config_t config = {
        .url = "http://43.201.16.59:8001/upload/sensor/1",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // // POST
    // // 현재 시간 가져오기
    ESP_LOGI(TAG, "Start Time");
    time_t timer = time(NULL);
    struct tm* t = localtime(&timer);
    int year = t->tm_year + 1900;
    int mon = t->tm_mon + 1;
    int day = t->tm_mday;
    int hour = t->tm_hour;
    int min_m = t->tm_min;
    int sec = t->tm_sec;
    int wday = t->tm_wday;

    // // 데이터 만들기
    // // "YYYY" : "2013"
    // // "MM" : "11"
    // // "DD" : "6"
    // // "HH" : "10"
    // // "Min" : "8"
    // // "Sec" : "45"
    // // "Day" : "0"
    // // "Illuminance" : "2022"
    // // "Manual" : "1"
    // // "Brightness" : "233"
    // // "On" : "0"
    
    char post_data[250] =  { 0, };
    sprintf(post_data, "{\"YYYY\" : \"%d\", \"MM\" : \"%d\", \"DD\" : \"%d\", \"HH\" : \"%d\", \"Min\" : \"%d\", \"Sec\" : \"%d\", \"Day\" : \"%d\", \"Illuminance\" : \"%d\", \"Manual\" : \"%d\", \"Brightness\" : \"%d\", \"On\" : \"%d\"}", year, mon, day, hour, min_m, sec, wday, illuminance, is_auto, brightness, is_on);
    
    // const char *post_data = "hihi!!";
    // ESP_LOGI(TAG, "create time_string");
    // printf("hihi!!\n");
    printf(post_data);
    printf("\n");
    esp_http_client_set_url(client, "http://43.201.16.59:8001/upload/sensor/M16M");
    ESP_LOGI(TAG, "set_url");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    ESP_LOGI(TAG, "set_method");
    esp_http_client_set_header(client, "Content-Type", "application/json");
    ESP_LOGI(TAG, "set_header");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    ESP_LOGI(TAG, "set post field");
    esp_err_t err = esp_http_client_perform(client);
    ESP_LOGI(TAG, "set http config");
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGI(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    // POST
    // const char *post_data = "{\"field1\":\"value1\"}";
    // esp_http_client_set_url(client, "http://43.201.16.59:8000/upload/sensor/1");
    // esp_http_client_set_method(client, HTTP_METHOD_POST);
    // esp_http_client_set_header(client, "Content-Type", "application/json");
    // esp_http_client_set_post_field(client, post_data, strlen(post_data));
    // esp_err_t err = esp_http_client_perform(client);
    // if (err == ESP_OK) {
    //     ESP_LOGI(TAG, "HTTP POST Status = %d", esp_http_client_get_status_code(client));
    // } else {
    //     ESP_LOGI(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    // }

    esp_http_client_cleanup(client);
}