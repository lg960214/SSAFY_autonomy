from fastapi import FastAPI, APIRouter
from fastapi.responses import FileResponse


#fileURL = "실제 스토리지의 경로"


PthFileRouter = APIRouter(prefix="/download/pthfile")

@PthFileRouter.get("/{productionName}/{order}", tags=["pthfile"])
def getPthFile(productionName, order):
    fileURL = f"/code/app/PthStorage/{productionName}"
    try :
        fHandler = open(fileURL, 'r')
        fHandler.seek(int(order)*2044)
        data = fHandler.read(2044)
        if data =="":
            print("End of File")
            return "1"
            
        result = "0" + data 
        print(f"productionName : {productionName}, filename : {fileURL[fileURL.rfind('/')+1:]}")
        return result
    except FileNotFoundError:
        print('There is no file') #-> 디버깅 용
        return "2"
    else : 
        fHandler.close()
    #return FileResponse(fileURL)   #-> 디버깅용
