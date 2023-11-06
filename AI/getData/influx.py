import influxdb_client
import pandas as pd

bucket = "MaiL"

myDf = pd.DataFrame()

# conffig_path = "./config.ini"
# input your setting in config.ini file
client = influxdb_client.InfluxDBClient.from_config_file("./config.ini")

query_api = client.query_api()

# start: -1d == one day
query = f'from(bucket:"MaiL")\
        |> range(start:-1d)\
        |> filter(fn:(r)=> r._measurement == "SensorData" and r.Name == "M16M")'

result = query_api.query(query=query)

for table in result:
    col = table.records[0].get_field()
    temp = []
    for record in table.records:
        temp.append((int(record.get_value())))
    myDf[col] = temp

print(myDf)
mdata = myDf[["YYYY", "MM", "Day", "DD", "HH", "Min", "Sec",
    "Illuminance", "Manual", "On", "Brightness"]]
print("Update order")
print(mdata)


'''
config.ini
url < input your DB address ! 
'''
'''
[influx2]
url=http://172.17.0.3:8086
org=Webx
token=8A80B1097C150034D
timeout=6000
verify_ssl=False
'''