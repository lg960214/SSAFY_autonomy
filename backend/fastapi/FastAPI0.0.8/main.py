from typing import Union
from fastapi import FastAPI
from .router.RawDataGet import SensorRouter
from .router.PthFileSend import PthFileRouter


app = FastAPI()


app.include_router(SensorRouter)
app.include_router(PthFileRouter)
