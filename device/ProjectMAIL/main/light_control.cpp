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
    static node_t *node = node::get();
    static endpoint_t *endpoint_id = endpoint::get(node, 1); // get(node, endpoint_id);;
    cluster_t *cluster_id = cluster::get(endpoint_id, OnOff::Id);
    attribute_t *attribute_id = attribute::get(cluster_id, OnOff::Attributes::OnOff::Id);
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute_id, &val);
    
    val.val.b = power;

    err = led_driver_set_power(light_handle, power);
    attribute::update(1, *cluster_id, *attribute_id, &val);    
    return err;
}                         

esp_err_t light_set_brightness(uint8_t target) // 0~255
{
    esp_err_t err = ESP_OK;
    static node_t *node = node::get();
    static endpoint_t *endpoint_id = endpoint::get(node, 1); // get(node, endpoint_id);;
    cluster_t *cluster_id = cluster::get(endpoint_id, LevelControl::Id);
    attribute_t *attribute_id = attribute::get(cluster_id, LevelControl::Attributes::CurrentLevel::Id);
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute_id, &val);
    
    val.val.u8 = target;
    int value = REMAP_TO_RANGE(val.val.u8, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);

    err = led_driver_set_brightness(light_handle, value);
    attribute::update(1, *cluster_id, *attribute_id, &val);   
    return err;
}

bool light_get_power()
{
    static node_t *node = node::get();
    static endpoint_t *endpoint_id = endpoint::get(node, 1);
    cluster_t *cluster_id = cluster::get(endpoint_id, OnOff::Id);
    attribute_t *attribute_id = attribute::get(cluster_id, OnOff::Attributes::OnOff::Id);
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute_id, &val);

    return val.val.b; 
}

int8_t light_get_brightness()
{
    static node_t *node = node::get();
    static endpoint_t *endpoint_id = endpoint::get(node, 1);
    cluster_t *cluster_id = cluster::get(endpoint_id, LevelControl::Id);
    attribute_t *attribute_id = attribute::get(cluster_id, LevelControl::Attributes::CurrentLevel::Id);
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute_id, &val);

    return val.val.u8;
}

