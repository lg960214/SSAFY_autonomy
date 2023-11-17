import influxdb_client
import pandas as pd
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

def getData(bucket, config_path):
    retDf = pd.DataFrame()
    colName = []
    client = influxdb_client.InfluxDBClient.from_config_file(config_path)
    query_api = client.query_api()
    query = f'from(bucket:"{bucket}")\
    |> range(start:-1d)\
    |> filter(fn:(r)=> r._measurement=="SensorData" and r.Name=="M16M")'

    result = query_api.query(query=query)

    for table in result:
        col = table.records[0].get_field()
        colName.append(col)
        temp = []
        for record in table.records:
            temp.append((int(record.get_value())))
        retDf[col] = temp
    
    retDf = retDf[["MM", "DD", "Day", "HH", "Min", "Sec",
    "Illuminance", "Movement", "Manual", "On",
     "Brightness"]]

    return retDf

def dataPreprocessing(dataTable):
    myData = dataTable.values
    feature_size = myData.shape[1] - 1
    return train_test_split(myData[:,:feature_size], myData[:,feature_size:],
                                                    test_size=0.1, random_state=42,
                                                   shuffle=False)

def modelLoad(model_path):
    pre_model = tf.keras.models.load_model(model_path)
    return pre_model

def modelTransferTrain(x_train, y_train, pre_model):
    model = tf.keras.models.Sequential()

    pre_feat = pre_model.layers[0].input.shape[1]
    new_feat = x_train[1].shape[0]
    feat_change = False
    
    if pre_feat == new_feat:
        model.add(pre_model.layers[0])
    else:
        model.add(tf.keras.layers.Dense(16, activation = 'relu', input_shape = (new_feat, )))
        feat_change = True
    
    for i in range(1, len(pre_model.layers)):
        model.add(pre_model.layers[i])
    for i in range(len(pre_model.layers) - 2):
        if feat_change and i == 0: continue
        model.layers[i].trainable = False
    
    model.compile(optimizer = 'adam', loss = 'mse')
    '''
    # Confirm model Architect
    print(model.summary())
    for layers in model.layers:
        print(layer, layer.trainable)
    '''
    history = model.fit(x_train, y_train,
    epochs = 10,
    batch_size = 32,
    validation_split = 0.18)

    # Visualization
    plt.plot(history.history['loss'], label='Train Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.legend()
    plt.show()

    return model

def modelTest(x_test, y_test, model, model_path=None):
    score = model.evaluate(x_test, y_test)
    
    # Visualization
    y_pred = model.predict(x_test)
    x_label = [x for x in range(0, x_test.shape[0])]
    plt.scatter(x_label, y_test, label = "Actual", s = 0.01)
    plt.scatter(x_label, y_pred, label = "Predicted", s = 0.01)
    plt.xlabel('Sample count')
    plt.ylabel('Value')
    plt.legend()
    plt.show()

    # Save model
    # if ...
    if model_path != None:
        model.save(model_path)

    return score

def modelConvert(model_source, file_path):
    if type(model_source) == str:
        model = tf.keras.models.load_model(model_source)
    else:
        model = model_source
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()
    with open(file_path, 'wb') as f:
        f.write(tflite_model)
    print("success")
    return