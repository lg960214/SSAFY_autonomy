from fastapi import FastAPI,APIRouter
from typing import Union
from pydantic import BaseModel
import datetime as dt
import os
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "MaiL"


client = influxdb_client.InfluxDBClient.from_config_file("/code/app/config.ini")

write_api = client.write_api(write_options=SYNCHRONOUS)


class SensorRawData(BaseModel):
    MM : Union[int, None] = None
    DD : Union[int, None] = None
    HH : Union[int, None] = None
    Min : Union[int, None] = None
    Sec : Union[int, None] = None
    Day : Union[int, None] = None 
    Illuminance : Union[int, None] = None
    Manual : Union[bool, None] = None
    Brightness : Union[int, None] = None
    On : Union[bool, None] = None


SensorRouter = APIRouter(prefix="/upload/sensor")

@SensorRouter.post("/{productionName}", tags=["sensor"])
def postSensorData(productionName,sensorData: SensorRawData):
    try :
        print(sensorData)

        p = influxdb_client.Point("SensorData").tag("Name", productionName).field("MM",sensorData.MM).field("DD",sensorData.DD).field("HH",sensorData.HH).field("Min",sensorData.Min).field("Sec",sensorData.Sec).field("Day",sensorData.Day).field("Illuminance",sensorData.Illuminance).field("Manual",sensorData.Manual).field("Brightness",sensorData.Brightness).field("On",sensorData.On)
        write_api.write(bucket=bucket, record=p)
        print("Well Done")
     
        
        return "OK"
    except Exception as ex :
        errMsg = traceback.format_exc()
        print(errMsg)
        return "Not Good"
     

