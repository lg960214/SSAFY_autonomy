import MaiL
# 1. get data from influxDB
bucket = "MaiL"
config_path = "./config.ini"
data = MaiL. getData(bucket, config_path)
# 2. Data preprocessing
x_train, x_test, y_train, y_test = MaiL.dataPreprocessing(data)
# 3. Load pre_model
model_path = "./DNN_231113"
pre_model = MaiL.modelLoad(model_path)
# 4. Transfer learning
model = MaiL.modelTransferTrain(x_train, y_train, pre_model)
# 5. Test
print(f'test loss : {MaiL.modelTest(x_test, y_test, model)}')
# 6. Convert to tflite
file_path = "./DNN_231113.tflite"
MaiL.modelConvert(model, file_path)