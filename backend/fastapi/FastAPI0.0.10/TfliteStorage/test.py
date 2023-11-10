#from fastapi import FastAPI, APIRouter
#from minio import Minio
#from minio.error import S3Error
import subprocess
import codecs


#from fastapi.responses import FileResponse





#TfliteFileRouter = APIRouter(prefix="/download/tflitefile")
#minioClient = Minio("ip:port", access_key="access_key", secret_key="secret_key", secure=False)


#@TfliteFileRouter.get("/{productionName}/{order}", tags=["tflitefile"])
def getTfliteFile(order):
    fileURL = f"M16M.tflite"
    try :
        #minioClient.fget_object("bucket name", "object file name", f"/code/app/TfliteStorage/{productionName}.tflite")
        data = 0
        with open(fileURL, 'rb') as fHandler:
            fHandler.seek(order*61)
            data = fHandler.read(61)
            
        if not data:
            print("End of File")
            #subprocess.run(['rm','M16M.tflite'])
            print("1")

        result = data
        #print(f"productionName : {productionName}, filename : {fileURL[fileURL.rfind('/')+1:]}")
        print(result)
#    except S3Error as err:
#        print(err)
#        print("2")
    except FileNotFoundError:
        print('There is no file')
        print("2")

    #return FileResponse(fileU:RL)   #-> 디버깅용


for j in range(10000):
    getTfliteFile(j)

