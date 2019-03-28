import datetime
import serial
from influxdb import InfluxDBClient

initTime = datetime.datetime.now()

client = InfluxDBClient(host='localhost', port=8086)
client.switch_database('mytest')

arduino = serial.Serial('COM10', 9600)
json_body = [
    {
        "measurement": "biogas",
        "tags": {
            "month": initTime.month,
            "day": initTime.day,
            "bottle": "1"
        },
        "fields": {
            "temperature": 37,
            "pH": 5
        }
    }
]

result = client.query('SELECT * FROM "mytest"."autogen"."biogas"')
print(result.raw)

while True:
    while arduino.in_waiting:
        _json = arduino.readline()
        client.write_points(_json)