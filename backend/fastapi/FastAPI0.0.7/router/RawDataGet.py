from fastapi import FastAPI,APIRouter
#from typing import Optional
from pydantic import BaseModel
import datetime as dt
import os
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "MaiL"


client = influxdb_client.InfluxDBClient.from_config_file("/code/app/config.ini")

#retention_rules = [{'type': 'expire', 'everySeconds': 86400, 'shardGroupDurationSeconds': 86400, 'name': '7d'}]

#client.buckets_api().update_bucket(bucket=bucket, retention_rules=retention_rules)
#Write script
write_api = client.write_api(write_options=SYNCHRONOUS)


class SensorRawData(BaseModel):
    data: str

SensorRouter = APIRouter(prefix="/upload/sensor")

@SensorRouter.post("/{productionName}", tags=["sensor"])
def postSensorData(productionName,sensorData: SensorRawData):
    try :
        print(sensorData.data)
        c = sensorData.data.split(',')

        p = influxdb_client.Point("SensorData").tag("Name", productionName).field("YYYY", int(c[0])).field("MM",int(c[1])).field("DD",int(c[2])).field("HH",int(c[3])).field("Min",int(c[4])).field("Sec",int(c[5])).field("Day",int(c[6])).field("Brightness",int(c[7])).field("Auto",int(c[8])).field("On",int(c[9])).field("illuminance",int(c[10]))
        write_api.write(bucket=bucket, record=p)
        print(c)

        
        query_api = client.query_api()
        query = f'from(bucket:"MaiL")\
                |> range(start: -10m)\
                |> filter(fn:(r) => r._measurement == "SensorData" and r.Name == "{productionName}")'
        result = query_api.query(query=query)
        for table in result:
            print(table)
            for record in table.records:
                print(record.get_field(),record.get_value())
     

        return "OK"
    except :
        return "Not Good"
#    p = influxdb_client.Point("CommuTest").tag("data", "esp32").field("string", item.data)
#    write_api.write(bucket=bucket, record=p)
    # Query script
#    query_api = client.query_api()
#    query = 'from(bucket:"SensorData")\
#    |> range(start: -10m)\
#    |> filter(fn:(r) => r._measurement == "CommuTest")'
#    result = query_api.query(query=query)
#    for table in result:
#        print(table)
#        for record in table.records:
#            print(record["_time"], record["data"], record["_value"])
     

