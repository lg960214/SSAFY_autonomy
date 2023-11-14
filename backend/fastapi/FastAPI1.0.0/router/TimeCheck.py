from fastapi import FastAPI, APIRouter
import time



TimeCheckRouter = APIRouter(prefix="/MaiL")

@TimeCheckRouter.get("/gettime", tags=["MaiL"])
def getTimeFile():
    try :
        return int(time.time())
    except :
        return 1
