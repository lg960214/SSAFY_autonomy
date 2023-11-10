from typing import Union
from fastapi import FastAPI
from .router.RawDataGet import SensorRouter
from .router.TfliteFileSend import TfliteFileRouter
from .router.TimeCheck import TimeCheckRouter


app = FastAPI()


app.include_router(SensorRouter)
app.include_router(TfliteFileRouter)
app.include_router(TimeCheckRouter)
