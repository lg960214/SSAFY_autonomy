from fastapi import FastAPI, APIRouter
from minio import Minio
from minio.error import ResponseError
import subprocess
#from fastapi.responses import FileResponse





TfliteFileRouter = APIRouter(prefix="/download/tflitefile")
minioClient = Minio("ip:port", access_key="access_key", secret_key="secret_key", secure=False)


@TfliteFileRouter.get("/{productionName}/{order}", tags=["tflitefile"])
def getTfliteFile(productionName, order):
    fileURL = f"/code/app/TfliteStorage/{productionName}.tflite"
    try :
        minioClient.fget_object("bucket name", "object file name", f"/code/app/TfliteStorage/{productionName}.tflite")
        data = b""
        with open(fileURL, 'rb') as fHandler:
            fHandler.seek(int(order)*2044)
            data = fHandler.read(2044)
        if data == b"":
            print("End of File")
            subprocess.run(['rm',f'/code/app/TfliteStorage/{productionName}'])
            return "1"
            
        result = "0" + data 
        print(f"productionName : {productionName}, filename : {fileURL[fileURL.rfind('/')+1:]}")
        return result
    except ResponseError as err:
        print(err)
        return "2"
    except FileNotFoundError:
        print('There is no file') 
        return "2"
        
    #return FileResponse(fileU:RL)   #-> 디버깅용
