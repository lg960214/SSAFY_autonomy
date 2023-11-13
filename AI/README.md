## MaiL API

1. GetData
- input : bucket name, `config.ini` file path
- output : Data table(pandas dataframe)

2. DataPreprocessing
- input : Data table
- output : Data table (x_train, x_test, y_train, y_test; type : numpy array)

3. ModelLoad
- input :  model path
- output : tensorflow model instance

4. ModelTransferTrain
- input : preprocessed Data, tensorflow model instance
- output : model instance ( + valid score)

5. ModelTest
- input : test data, model instance (+ model path)
- output : test score (+ model dir)

6. ModelConvert
- input : model path or model instance, tflite file path
- output : -

> ( Performed at first time ONLY )<br>
    3. ModelCreate<br>
    input : - <br>
    output : tensorflow model instance<br><br>
    4. ModelTrain<br>
    input : tensorflow model instance<br>
    output : tf model dir<br><br>