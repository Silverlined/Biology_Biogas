import datetime
import serial
from influxdb import InfluxDBClient

initTime = datetime.datetime.now()

client = InfluxDBClient(host='localhost', port=8086)
# client.drop_database('mytest')
# client.create_database('mytest')
client.switch_database('mytest')

port = 'COM4'
arduino = serial.Serial(port, 9600)

def createJson(temp, pH, volume, n):
    json_body = [
            {
                "measurement": "biogas",
                "tags": {
                    "month": initTime.month,
                    "day": initTime.day,
                    "bottle": str(n)
                },
                "fields": {
                    "temperature": temp,
                    "pH": pH,
                    "volume": volume
                }
            }
        ]
    return json_body

result = client.query('SELECT * FROM "mytest"."autogen"."biogas"')
print(result.raw)

while True:
    while arduino.in_waiting:
        _str = str(arduino.readline())
        _str = _str.replace('b\'', '')
        _str = _str.replace('\\r\\n\'', '')
        params = [x.strip() for x in _str.split(',') if x != ""]
        _json = createJson(float(params[0]), float(params[1]), float(params[2]), 1)
        _json2 = createJson(0.0, 0.0, float(params[3]), 2)
        client.write_points(_json, time_precision='m')
        client.write_points(_json2, time_precision='m')