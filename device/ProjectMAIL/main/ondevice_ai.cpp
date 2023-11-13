/* EXAMPLE */

#include "ondevice_ai.h"

#include "dirent.h"

#define INPUT_DIM 1
#define OUTPUT_DIM 1
#define BATCH_SIZE 1
#define INPUT_SEQ_LEN 10

const char* TAG = "ondevice_ai";

namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input0 = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 10240;
uint8_t tensor_arena[kTensorArenaSize];
}

const float kXrange = 2.f * 3.14159265359f;
const int kInferencesPerCycle = 20;

void setModel() {
    // SPIFFS Configuration
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 1,
      .format_if_mount_failed = true
    };

    // Initialize SPIFFS
    ESP_LOGI(TAG, "Load SPIFFS");
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPIFFS");
        return;
    }

    // Read the .tflite model from SPIFFS
    FILE* file = fopen("/spiffs/model.tflite", "rb");
    if (file == NULL) {
        ESP_LOGE("SPIFFS", "Failed to open model file");
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t modelSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // load model to buffer
    uint8_t* modelBuffer = (uint8_t*) malloc(modelSize);
    fread(modelBuffer, 1, modelSize, file);
    fclose(file);

    // Get model from buffer
    model = tflite::GetModel(modelBuffer);

    esp_vfs_spiffs_unregister(NULL); // Unregist SPIFFS

    /* Load from cc file */
    // ESP_LOGI(TAG, "Load model");
    // model = tflite::GetModel("model.cc");

    static tflite::MicroMutableOpResolver<1> resolver;
    TfLiteStatus resolver_status = resolver.AddFullyConnected();

    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();

    input0 = interpreter->input(0);
    output = interpreter->output(0);
}

uint8_t inference(int illuminance, bool is_auto, int brightness, int motion, struct tm t) {
    /*
    Input data
    "MM", "DD", "Day", "HH", "Min", "Sec",
    "Illuminance", "Movement", "Manual", "On"
    */

   int is_on = brightness? true : false;
   int inputs[INPUT_SEQ_LEN] = {t.tm_mon + 1, t.tm_mday, t.tm_wday, t.tm_hour, t.tm_min, t.tm_sec, illuminance, motion, is_auto, is_on};
   memcpy(input0->data.i32, inputs, sizeof(int) * INPUT_SEQ_LEN);

    interpreter->Invoke();

    uint8_t result = (uint8_t)output->data.f[0];
    if(result < 10) result = 0;

    ESP_LOGI(TAG, "Inferred %u", result);
    return result;
}