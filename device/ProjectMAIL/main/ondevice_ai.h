#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include <esp_log.h>

// std::unique_ptr<tflite::FlatBufferModel> loadModelFromSPIFFS(const char* model_path);
void printTest();