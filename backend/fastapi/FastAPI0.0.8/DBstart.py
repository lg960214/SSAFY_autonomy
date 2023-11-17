import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
from influxdb_client.client.delete_api import DeleteApi

bucket = "MaiL"

client = influxdb_client.InfluxDBClient.from_config_file("/code/app/config.ini")
write_api = client.write_api(write_options=SYNCHRONOUS)

measurement = "SensorData"
fields = {
    "YYYY" : int,
    "MM" : int,
    "DD" : int,
    "HH" : int,
    "Min" : int,
    "Sec" : int,
    "Day" : int,
    "Brightness" : int,
    "Auto" : bool,
    "On" : bool,
    "illuminance" : int
}

mailSchema = f'{measurement},YYYY={fields["YYYY"]},MM={fields["MM"]},DD={fields["DD"]},HH={fields["HH"]},Min={fields["Min"]},Sec={fields["Sec"]},Day={fields["Day"]},Brightness={fields["Brightness"]},Auto={fields["Auto"]},On={fields["On"]},illuminance={fields["illuminance"]}'
createQuery = f'CREATE DATABASE {bucket} WITH DURATION 1w NAME "{mailSchema}"'

#data_point = Point(measurement) \
#    .time("2023-11-02T12:00:00Z") \  # 시간을 설정하세요
#    .field("temperature", 25.5) \  # 온도를 설정하세요
#    .field("humidity", 60)  # 습도를 설정하세요
client.query_api().query(query=createQuery)
#write_api.write(bucket=bucket, record=data_point)

# 클라이언트 종료
client.close()

