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
// TfLiteTensor* input1 = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 3000;
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
        ESP_LOGE(TAG, "Failed to initialize SPIFFS");
        return;
    }
    
    // // DEBUG check file lists
    // ESP_LOGI(TAG, "Open directory");
    // DIR *dir = opendir("/spiffs/");
    
    // ESP_LOGI("SPIFFS", "List of files:");
    // struct dirent *ent;
    // while ((ent = readdir(dir)) != NULL) {
    //     ESP_LOGI("SPIFFS", "  %s", ent->d_name);
    // }
    // closedir(dir);

    // Read the .tflite model from SPIFFS
    FILE* file = fopen("/spiffs/model_low2.tflite", "rb");
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

    ESP_LOGI(TAG, "Unregist SPIFFS");
    esp_vfs_spiffs_unregister(NULL); // Unregister SPIFFS

    /* Load from cc file */
    // ESP_LOGI(TAG, "Load model");
    // model = tflite::GetModel("model.cc");

    ESP_LOGI(TAG, "Make resolver");
    static tflite::MicroMutableOpResolver<1> resolver;
    resolver.AddFullyConnected();

    ESP_LOGI(TAG, "Make interpreter");
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    ESP_LOGI(TAG, "Allocate tensors");
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    ESP_LOGI(TAG, "Allocate status : %d", allocate_status);

    ESP_LOGI(TAG, "Set input");
    input0 = interpreter->input(0);
    // input1 = interpreter->input(1);

    ESP_LOGI(TAG, "Set output");
    output = interpreter->output(0);

    // Keep track of how many inferences we have performed.
    inference_count = 0;
}

uint8_t inference() {
    ESP_LOGI(TAG, "Inference");

    input0->data.f[0] = 315.0f;
    input0->data.f[1] = 10000.0f;
    input0->data.f[2] = 0.0f;
    input0->data.f[3] = 892.0f;

    // float sample_0[4] = {315, 10000, 0, 892};
    // float sample_1[10] = {69, 69, 69, 69, 69, 69, 69, 69, 69, 69}; // 센서 데이터

    // ESP_LOGI(TAG, "interpreter->typed_input_tensor");
    // float* input_data_0 = interpreter->typed_input_tensor<float>(0);
    // float* input_data_1 = interpreter->typed_input_tensor<float>(1);

    // ESP_LOGI(TAG, "insert sample data");
    // for (int i = 0; i < 4; i++) {
    //     input_data_0[i] = sample_0[i];
    // }

    // for (int i = 0; i < 10; i++) {
    //     input_data_1[i] = sample_1[i];
    // }

    // Invoke inference
    ESP_LOGI(TAG, "Invoke");
    interpreter->Invoke();


    // Get the output tensor
    // ESP_LOGI(TAG, "interpreter->typed_output_tensor");
    // float* output_data = interpreter->typed_output_tensor<float>(0);

    // Print the output data
    ESP_LOGI(TAG, "insert output data");
    for (int i = 0; i < OUTPUT_DIM; i++) {
        ESP_LOGI(TAG, "Output[%d] = %f", i, output->data.f[i]);
    }

    uint8_t result = (uint8_t)output->data.f[0];
    if(result < 10) result = 0;

    return result;
}