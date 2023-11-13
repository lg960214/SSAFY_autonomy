#include <stdio.h>
#include <sys/param.h>
#include "esp_log.h"
#include "esp_tls.h"
#include "http_client.h"

#include "esp_spiffs.h"

#include "mbedtls/base64.h"

static const char* TAG = "HTTP_CLIENT";
#define MAX_HTTP_OUTPUT_BUFFER 100

static void base64_decode_to_file(const char *base64_string, const char *file_path) {
    size_t output_size;
    mbedtls_base64_decode(NULL, 0, &output_size, (const unsigned char *)base64_string, strlen(base64_string));

    uint8_t *binary_data = (uint8_t *)malloc(output_size);
    mbedtls_base64_decode(binary_data, output_size, &output_size, (const unsigned char *)base64_string, strlen(base64_string));

    FILE *file = fopen(file_path, "ab");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        free(binary_data);
        return;
    }

    fwrite(binary_data, 1, output_size, file);
    fclose(file);

    ESP_LOGI(TAG, "Base64 decoded data written to file: %s", file_path);

    free(binary_data);
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            break;
        case HTTP_EVENT_ON_CONNECTED:
            break;
        case HTTP_EVENT_HEADER_SENT:
            break;
        case HTTP_EVENT_ON_HEADER:
            break;
        case HTTP_EVENT_ON_DATA:
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}

void send_sensor_data(int illuminance, bool is_auto, int brightness, struct tm t)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    esp_http_client_config_t config = {
        .url = "http://43.201.16.59:8001/upload/sensor/1",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    char post_data[250] =  { 0, };
    int is_on = brightness? true : false;
    sprintf(post_data, "{\"MM\" : \"%d\", \"DD\" : \"%d\", \"HH\" : \"%d\", \"Min\" : \"%d\", \"Sec\" : \"%d\", \"Day\" : \"%d\", \"Illuminance\" : \"%d\", \"Manual\" : \"%d\", \"Brightness\" : \"%d\", \"On\" : \"%d\"}", t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, t.tm_wday, illuminance, is_auto, brightness, is_on);
    esp_http_client_set_url(client, "http://43.201.16.59:8001/upload/sensor/M16M");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_err_t err = esp_http_client_perform(client);
    ESP_LOGI(TAG, "Post Sensor Data");
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGI(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

void get_tflite_file(void)
{
        char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    esp_http_client_config_t config = {
        .host = "http://43.201.16.59:8001/download/tflitefile/M16M/",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET
    int status_code = 0;
    int order_code = -1;
    int host_url_len = strlen(config.host);
    char current_url[100] = { 0, };
    char order_str[30] = { 0, };

    strcpy(current_url, config.host);

    // file systema init
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 1,
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

    // reset file
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/model.tflite", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fclose(f);

    // data request start
    esp_err_t err = ESP_OK;

    while (err == ESP_OK && status_code == 0)
    {
        while (status_code == 0)
        {
            // file read continue.
            // set url
            order_code += 1;
            memset(order_str, 0, sizeof(order_str));
            itoa(order_code, order_str, 10);
            strcpy(&current_url[host_url_len], order_str);
            esp_http_client_set_url(client, current_url);

            // get request
            err = esp_http_client_perform(client);
            if (err != ESP_OK)
            {
                ESP_LOGI(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
                break;
            }

            ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
            
            // file write

            local_response_buffer[esp_http_client_get_content_length(client) - 1] = '\0';
            const char *file_path = "/spiffs/model.tflite";

            base64_decode_to_file(&local_response_buffer[2], file_path);

            status_code = local_response_buffer[1] - '0';
        }
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            break;
        }
    }

    esp_vfs_spiffs_unregister(NULL);
    esp_http_client_cleanup(client);
}