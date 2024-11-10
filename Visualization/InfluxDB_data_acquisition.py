#参照元　https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v2/python/
#参照元 https://dev.classmethod.jp/articles/send-and-visualize-data-with-influxdb-cloud/
#https://docs.influxdata.com/influxdb/cloud-serverless/reference/client-libraries/v3/python/#installation

import os
import time
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "cansat"
org = "science"
token = os.getenv("INFLUXDB_TOKEN")
url="http://localhost:8086"

client = influxdb_client.InfluxDBClient(
    url=url,
    token=token,
    org=org
)

def get_imu_data():
    query_api = client.query_api()
    query = 'from(bucket: "cansat")\
            |> range(start: -10m)\
            |> filter(fn: (r) => r["_measurement"] == "cansat")\
            |> filter(fn: (r) => r["_field"] == "Accx" or r["_field"] == "Accy" or r["_field"] == "Accz" or r["_field"] == "Gyrx" or r["_field"] == "Gyry" or r["_field"] == "Gyrz" or r["_field"] == "Magx" or r["_field"] == "Magy" or r["_field"] == "Magz")'
    result = query_api.query(org=org, query=query)
    results = []
    for table in result:
        for record in table.records:
            results.append(record.get_value())
        return results