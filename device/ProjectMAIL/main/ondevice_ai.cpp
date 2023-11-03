#include "ondevice_ai.h"

const char* TAG = "ondevice_ai";

namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 2000;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

class FlatBufferModel {
  // Build a model based on a file. Return a nullptr in case of failure.
  static std::unique_ptr<FlatBufferModel> BuildFromFile(
      const char* filename,
      ErrorReporter* error_reporter);

  // Build a model based on a pre-loaded flatbuffer. The caller retains
  // ownership of the buffer and should keep it alive until the returned object
  // is destroyed. Return a nullptr in case of failure.
  static std::unique_ptr<FlatBufferModel> BuildFromBuffer(
      const char* buffer,
      size_t buffer_size,
      ErrorReporter* error_reporter);
};

std::unique_ptr<tflite::FlatBufferModel> loadModelFromSPIFFS(const char* model_path) {

    // SPIFFS Configuration
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 1,
      .format_if_mount_failed = true
    };

    // Initialize SPIFFS
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Failed to initialize SPIFFS");
        return nullptr;
    }

    // Create the FlatBufferModel object directly from file
    tflite::StderrReporter error_reporter;
    auto model = tflite::FlatBufferModel::BuildFromFile(model_path, &error_reporter);

    if (!model) {
        ESP_LOGE("TFLite", "Failed to load model");
        esp_vfs_spiffs_unregister(NULL); // Unregister SPIFFS
        return nullptr;
    }

    esp_vfs_spiffs_unregister(NULL); // Unregister SPIFFS
    return model;
}

void printTest() {
    ESP_LOGI(TAG, "Hello");
}