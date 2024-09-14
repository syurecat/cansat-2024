
#参照元　https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v2/python/
#参照元 https://dev.classmethod.jp/articles/send-and-visualize-data-with-influxdb-cloud/
#https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v3/python/#installation

import time
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "cansat"
org = "science"
token = "Bui6iPWok68lbgks7SySIrPUZSZP4y75MF3VjVsirJ7hHgFPQqR9qKug6EdJKp2k0954BHMtHMYrkgTtRLeSig=="
url="http://localhost:8086"

temp, humi, pres = 0.0, 0.0, 0.0
Accx, Accy, Accz = 0.0, 0.0, 0.0
Gyrx, Gyry, Gyrz = 0.0, 0.0, 0.0
Magx, Magy, Magz = 0.0, 0.0, 0.0
lati, long, alti, stre = 0.0, 0.0, 0.0, 0.0

client = influxdb_client.InfluxDBClient(
    url=url,
    token=token,
    org=org
)

write_api = client.write_api(write_options=SYNCHRONOUS)

p = influxdb_client.Point("my_measurement").tag("location", "Prague").field("temperature", 25.3)
write_api.write(bucket=bucket, org=org, record=p)

#######################################################################################################

import serial

ser = serial.Serial('COM7',115200,timeout=None)

database="cansattable"

i=-1
while True: 
    line = ser.readline()
    line = line.decode('utf-8', errors='ignore').strip()
    print(line)
    row = line
    row = row.split(',')[1:]
    print(row)
    if "BME" in line :
        temp,humi,pres,stre = map(float, row)
    elif "ACC" in line:
        Accx,Accy,Accz,stre = map(float, row)        
    elif "GYR" in line:
        Gyrx,Gyry,Gyrz,stre = map(float, row)     
    elif "MAG" in line:
        Magx, Magy, Magz,stre = map(float, row)     
    elif "GPS" in line:
        lati,long,alti,stre = map(float, row)

    i+=1
    if i >= 5:
        data = {
            "temp": temp,
            "humi": humi,
            "pres": pres,
            "Accx": Accx,
            "Accy": Accy,
            "Accz": Accz,
            "Gyrx": Gyrx,
            "Gyry": Gyry,
            "Gyrz": Gyrz,
            "Magx": Magx,
            "Magy": Magy,
            "Magz": Magz,
            "lati": lati,
            "long": long,
            "alti": alti,
            "stre": stre,
        }

        point = (
            influxdb_client.Point('cansat')
            .field("temp",data["temp"])
            .field("HUMI",data["humi"])
            .field("PRE",data["pres"])
            .field("Accx",data["Accx"])
            .field("Accy",data["Accy"])
            .field("Accz",data["Accz"])
            .field("Gyrx",data["Gyrx"])
            .field("Gyry",data["Gyry"])
            .field("Gyrz",data["Gyrz"])
            .field("Magx",data["Magx"])
            .field("Magy",data["Magy"])
            .field("Magz",data["Magz"])
            .field("lati",data["lati"])
            .field("long",data["long"])
            .field("alti",data["alti"])
            .field("stre",data["stre"])
            )
        write_api.write(bucket=bucket, org=org, record=point)
        time.sleep(0.05)
        i = 0