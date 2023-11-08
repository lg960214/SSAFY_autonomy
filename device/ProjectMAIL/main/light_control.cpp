#include "light_control.h"

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "light_control";
extern uint16_t light_endpoint_id;
extern app_driver_handle_t light_handle;
/* Do any conversions/remapping for the actual value here */

esp_err_t light_set_power(bool power)
{
    esp_err_t err = ESP_OK;
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, 1); // get(node, endpoint_id);
    cluster_t *cluster = NULL;
    attribute_t *attribute = NULL;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    cluster = cluster::get(endpoint, OnOff::Id);
    attribute = attribute::get(cluster, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);
    
    val.val.b = power;

    if(power)
        ESP_LOGW(TAG, "Light ON");
    else
        ESP_LOGW(TAG, "Light OFF");

    err = led_driver_set_power(light_handle, power);
    attribute::update(1, *cluster, *attribute, &val);    
    return err;
}                         

esp_err_t light_set_brightness(uint8_t target)
{
    esp_err_t err = ESP_OK;
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, 1); // get(node, endpoint_id);
    cluster_t *cluster = NULL;
    attribute_t *attribute = NULL;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    cluster = cluster::get(endpoint, LevelControl::Id);
    attribute = attribute::get(cluster, LevelControl::Attributes::CurrentLevel::Id);
    attribute::get_val(attribute, &val);
    
    val.val.u8 = target;
     // 254*val/100
    int value = REMAP_TO_RANGE(val.val.u8, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
    ESP_LOGW(TAG, "Brightness Set as %u", val.val.u8);
    ESP_LOGW(TAG, "value is %d", value);
    attribute::update(1, *cluster, *attribute, &val);    
    return led_driver_set_brightness(light_handle, value);
}

bool light_get_power(uint16_t endpoint_id)
{
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = NULL;
    attribute_t *attribute = NULL;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

        /* Setting power */
    cluster = cluster::get(endpoint, OnOff::Id);
    attribute = attribute::get(cluster, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);

    return val.val.b;
}

int8_t light_get_brightness(uint16_t endpoint_id)
{
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = NULL;
    attribute_t *attribute = NULL;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    /* Setting brightness */
    cluster = cluster::get(endpoint, LevelControl::Id);
    attribute = attribute::get(cluster, LevelControl::Attributes::CurrentLevel::Id);
    attribute::get_val(attribute, &val);

    return val.val.u8;
}
