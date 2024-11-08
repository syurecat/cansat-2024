import influxdb_client_3 ,os ,time#influxDB cloud import
from influxdb_client_3 import InfluxDBClient3, Point, WriteOptions,write_client_options, SYNCHRONOUS #特定のもののみをimport
import time

client = InfluxDBClient3(host=f"https://us-east-1-1.aws.cloud2.influxdata.com",
                        database=f"cansattable",
                        token=f"PFbeV34P6tr9B4ZTOob8HF8zI4Nx876hCMJCReng58GDvEhhNxX0lFC0yhNkLPqqlFoDCe3Q16uMXer3gjuqYw==",
                        write_options=SYNCHRONOUS,
                        flight_client_options=None,
                        )

database="cansattable"

def get_imu_data():
    query_api = client.query_api()
    query = 'from(bucket: "cansat")\
            |> range(start: -10m)\
            |> filter(fn: (r) => r["_measurement"] == "cansat")\
            |> filter(fn: (r) => r["_field"] == "Accx" or r["_field"] == "Accy" or r["_field"] == "Accz" or r["_field"] == "Gyrx" or r["_field"] == "Gyry" or r["_field"] == "Gyrz" or r["_field"] == "Magx" or r["_field"] == "Magy" or r["_field"] == "Magz")'
    result = query_api.query(org=org, query=query)
    results = []
    for table in result
        for record in table.records:
            return record.get_value()