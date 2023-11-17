#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include <esp_spiffs.h>

#include <esp_log.h>
#include <esp_err.h>

void setModel();
uint8_t inference(int illuminance, bool is_auto, int brightness, int motion, struct tm t);