from fastapi import FastAPI, APIRouter
import time

#fileURL = "실제 스토리지의 경로"


TimeCheckRouter = APIRouter(prefix="/MaiL")

@TimeCheckRouter.get("/gettime", tags=["MaiL"])
def getTimeFile():
    try :
        return int(time.time())
    except :
        return 1
