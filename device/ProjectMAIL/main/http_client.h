#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "esp_http_client.h"

void send_sensor_data(int illuminance, int is_auto, int is_on, int brightness);
void get_ftlite_file(void);

esp_err_t _http_event_handler(esp_http_client_event_t *evt);

#endif