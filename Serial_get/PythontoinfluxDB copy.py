
#参照元　https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v2/python/
#参照元 https://dev.classmethod.jp/articles/send-and-visualize-data-with-influxdb-cloud/
#https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v3/python/#installation

import influxdb_client_3 ,os ,time#influxDB cloud import
from influxdb_client_3 import InfluxDBClient3, Point, WriteOptions,write_client_options, SYNCHRONOUS #特定のもののみをimport
import serial
import time

ser = serial.Serial('COM4',115200,timeout=None)


client = InfluxDBClient3(host=f"https://us-east-1-1.aws.cloud2.influxdata.com",
                        database=f"cansattable",
                        token=f"PFbeV34P6tr9B4ZTOob8HF8zI4Nx876hCMJCReng58GDvEhhNxX0lFC0yhNkLPqqlFoDCe3Q16uMXer3gjuqYw==",
                        write_options=SYNCHRONOUS,
                        flight_client_options=None,
                        )

database="cansattable"

i=0
while True: 
  line = ser.readline()
  line = line.decode('utf-8', errors='ignore')
  print(line)
  row = line
  row = row.split(',')[1:-1]
  print(row)
  if "BME" in line :
      temp,humi,pres = map(float, row)
  elif "ACC" in line:
      Accx,Accy,Accz = map(float, row)        
  elif "GYR" in line:
      Gyrx,Gyry,Gyrz = map(float, row)     
  elif "MAG" in line:
      Magx, Magy, Magz = map(float, row)     
  elif "GPS" in line:
      stre = row[-1].rstrip('\r\n')
      lati, long, alti = map(float,row)
      stre = float(stre)

  i+=1
  if i >= 5:
      data = {
            "point": {
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
        },
        }
      for key in data:
          point = (
              Point('cansat1')
              .field("temp",data[key]["temp"])
              .field("HUMI",data[key]["humi"])
              .field("PRE",data[key]["pres"])
              .field("Accx",data[key]["Accx"])
              .field("Accy",data[key]["Accy"])
              .field("Accz",data[key]["Accz"])
              .field("Gyrx",data[key]["Gyry"])
              .field("Gyrz",data[key]["Gyrz"])
              .field("Magx",data[key]["Magx"])
              .field("Magy",data[key]["Magy"])
              .field("Magz",data[key]["Magz"])
              .field("lati",data[key]["lati"])
              .field("long",data[key]["long"])
              .field("alti",data[key]["alti"])
              .field("stre",data[key]["stre"])
              )
          client.write(database=database, record=point)
          time.sleep(0.05)
      i = 0

      
