#include "ondevice_ai.h"

#include "dirent.h"

const char* TAG = "ondevice_ai";

namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 2000;
uint8_t tensor_arena[kTensorArenaSize];
}

const float kXrange = 2.f * 3.14159265359f;
const int kInferencesPerCycle = 20;

void setModel() {
    // SPIFFS Configuration
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    // Initialize SPIFFS
    ESP_LOGI(TAG, "Load SPIFFS");
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Failed to initialize SPIFFS");
        return;
    }
    
    ESP_LOGI(TAG, "Open directory");
    DIR *dir = opendir("/spiffs/");

    ESP_LOGI("SPIFFS", "List of files:");
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        ESP_LOGI("SPIFFS", "  %s", ent->d_name);
    }

    // Cleanup
    closedir(dir);

    // Read the .tflite model from SPIFFS
    FILE* file = fopen("/spiffs/hello_world_int8.tflite", "rb");
    if (file == NULL) {
        ESP_LOGE("SPIFFS", "Failed to open model file");
        return;
    }

    ESP_LOGI(TAG, "Fseek");
    fseek(file, 0, SEEK_END);
    size_t modelSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    ESP_LOGI(TAG, "ModelBuffer");
    uint8_t* modelBuffer = (uint8_t*) malloc(modelSize);
    fread(modelBuffer, 1, modelSize, file);
    fclose(file);

    ESP_LOGI(TAG, "Load model");
    model = tflite::GetModel(modelBuffer);

    ESP_LOGI(TAG, "Unregist model");
    esp_vfs_spiffs_unregister(NULL); // Unregister SPIFFS

    ESP_LOGI(TAG, "Make resolver");
    static tflite::MicroMutableOpResolver<1> resolver;
    resolver.AddFullyConnected();

    ESP_LOGI(TAG, "Make interpreter");
    static tflite::MicroInterpreter static_interpreter(
            model, resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    ESP_LOGI(TAG, "Allocate tensors");
    TfLiteStatus allocate_status = interpreter->AllocateTensors();

    ESP_LOGI(TAG, "Set input");
    input = interpreter->input(0);

    ESP_LOGI(TAG, "Set output");
    output = interpreter->output(0);

    ESP_LOGI(TAG, "Set inference_count");
    inference_count = 0;
}

void inference() {
    ESP_LOGI(TAG, "Inference");

    float position = static_cast<float>(inference_count) /
                    static_cast<float>(kInferencesPerCycle);
    float x = position * kXrange;

    // Quantize the input from floating-point to integer
    int8_t x_quantized = x / input->params.scale + input->params.zero_point;
    // Place the quantized input in the model's input tensor
    input->data.int8[0] = x_quantized;

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        MicroPrintf("Invoke failed on x: %f\n",
                                static_cast<double>(x));
        return;
    }

    // Obtain the quantized output from model's output tensor
    int8_t y_quantized = output->data.int8[0];
    // Dequantize the output from integer to floating-point
    float y = (y_quantized - output->params.zero_point) * output->params.scale;

    // Output the results. A custom HandleOutput function can be implemented
    // for each supported hardware target.
    ESP_LOGI(TAG, "%f %f", static_cast<double>(x), static_cast<double>(y));

    // Increment the inference_counter, and reset it if we have reached
    // the total number per cycle
    inference_count += 1;
    if (inference_count >= kInferencesPerCycle) inference_count = 0;

    return;
}