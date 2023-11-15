from fastapi import FastAPI, APIRouter
from minio import Minio
from minio.error import S3Error
import subprocess
#from fastapi.responses import StreamingResponse
import base64

class MinioClient():
    def __init__(self):
        self.client = Minio(
            "minio-service.kubeflow:9000",
            access_key="minio",
            secret_key="minio123",
            secure=False
        )
	
    # Bucket이 존재하는 지 확인 후, 존재하지 않으면 Bucket을 만듦
    def create_bucket(self,bucket_name):
        if self.client.bucket_exists(bucket_name):
            print(f"{bucket_name} exists")
        else:
            self.client.make_bucket(bucket_name)        
            print(f"{bucket_name} created")
    def get_object(self, bucket_name, object_name, file_name):
        self.client.fget_object(bucket_name, object_name, file_name)
    # fput_object 함수를 활용해 업로드
    def upload_file(self, bucket, minio_path, local_path):
        self.client.fput_object(bucket, minio_path, local_path)

    # 디렉토리 업로드 방법
    # Object를 반복적으로 Upload하는 방식을 취하고 있다.
    # os.path로 디렉토리 내용 확인 후, 파일이면 업로드하고 디렉토리면 재귀함수 호출하는 방식
    def upload_directory(self, bucket, minio_path, local_path):
        if os.path.isdir(local_path):
            items = os.listdir(local_path)
            items.sort()
            for item in items:
                item_local_path = os.path.join(local_path, item)
                item_minio_path = os.path.join(minio_path, item)
                if os.path.isfile(item_local_path):
                    self.upload_file(bucket, item_minio_path, item_local_path)
                elif os.path.isdir(item_local_path):
                    self.upload_directory(bucket, item_minio_path, item_local_path)


TfliteFileRouter = APIRouter(prefix="/download/tflitefile")
minioClient = MinioClient()
minioClient.create_bucket("mail")

@TfliteFileRouter.get("/{productionName}/{order}", tags=["tflitefile"])
def getTfliteFile(productionName, order):
    fileURL = f"/code/app/TfliteStorage/{productionName}.tflite"
    try :
        if int(order) == 0:
            minioClient.get_object("mail", f"{productionName}.tflite", fileURL)
        data = b""
        with open(fileURL, 'rb') as fHandler:
            fHandler.seek(int(order)*60)
            data = fHandler.read(60)
        if data == b"":
            print("End of File")
            subprocess.run(['rm',fileURL])
            return "1"

        result = "0" + base64.b64encode(data).decode('utf-8') 
        print(f"productionName : {productionName}, filename : {fileURL[fileURL.rfind('/')+1:]}")
        return result
    except S3Error as err:
        print(err)
        return "2"
    except FileNotFoundError:
        print('There is no file') 
        return "2"
        
