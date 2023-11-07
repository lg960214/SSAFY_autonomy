from typing import Union
from fastapi import FastAPI
from .router.RawDataGet import SensorRouter
from .router.TfliteFileSend import TfliteFileRouter


app = FastAPI()


app.include_router(SensorRouter)
app.include_router(TfliteFileRouter)
