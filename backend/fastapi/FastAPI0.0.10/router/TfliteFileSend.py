from fastapi import FastAPI, APIRouter
from minio import Minio
from minio.error import S3Error
import subprocess
from fastapi.responses import StreamingResponse





TfliteFileRouter = APIRouter(prefix="/download/tflitefile")
#minioClient = Minio("ip:port", access_key="access_key", secret_key="secret_key", secure=False)


@TfliteFileRouter.get("/{productionName}/{order}", tags=["tflitefile"])
def getTfliteFile(productionName, order):
    fileURL = f"/code/app/TfliteStorage/{productionName}.tflite"
    try :
        #minioClient.fget_object("bucket name", "object file name", f"/code/app/TfliteStorage/{productionName}.tflite")
        data = b""
        with open(fileURL, 'rb') as fHandler:
            fHandler.seek(int(order)*61)
            data = fHandler.read(61)
        if data == b"":
            print("End of File")
            #subprocess.run(['rm',f'/code/app/TfliteStorage/{productionName}.tflite'])
            return StreamingResponse(iter([b"1"]), media_type="application/octet-stream")
            
        result = b"0" + data 
        print(f"productionName : {productionName}, filename : {fileURL[fileURL.rfind('/')+1:]}")
        return StreamingResponse(iter([result]), media_type="application/octet-stream")
    except S3Error as err:
        print(err)
        return StreamingResponse(iter([b"2"]), media_type="application/octet-stream") 
    except FileNotFoundError:
        print('There is no file') 
        return StreamingResponse(iter([b"2"]), media_type="application/octet-stream")
        
    #return FileResponse(fileU:RL)   #-> 디버깅용
