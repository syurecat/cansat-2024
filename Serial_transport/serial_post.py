import os
import time
import serial
import json
import configparser
import asyncio
import logging
import logging.handlers
from visualization_post import PostData

config = configparser.ConfigParser()
config.read("config.ini")

API_TOKEN = "cansat"
API_URL = config["api"]["api_url"]
DEVICE_NAME = config["senser"]["name"]
SERIAL_PORT = config["senser"]["serial_port"]
SERIAL_BAUDRATE = int(config["senser"]["serial_baudrate"])
post_client = PostData(API_TOKEN, API_URL, DEVICE_NAME)

logger = logging.getLogger('serial_post')
logger.setLevel(logging.DEBUG)
handler = logging.handlers.RotatingFileHandler(
    filename='discord.log',
    encoding='utf-8',
    maxBytes=32 * 1024 * 1024,
    backupCount=7,
)
dt_fmt = '%Y-%m-%d %H:%M:%S'
formatter = logging.Formatter('[{asctime}] [{levelname:<8}] {name}: {message}', dt_fmt, style='{')
handler.setFormatter(formatter)
logger.addHandler(handler)

async def main():
    await post_client.init()
    with serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1) as ser:
        try:
            while True:
                line = ser.readline().decode('utf-8').strip()
                if not line:
                    continue
                row = line.split(',')[1:]
                if "BME" in line:
                    sensor_type = "BME"
                elif "ACC" in line:
                    sensor_type = "ACC"
                elif "GYR" in line:
                    sensor_type = "GYR"
                elif "MAG" in line:
                    sensor_type = "MAG"
                elif "GPS" in line:
                    sensor_type = "GPS"

                if sensor_type:
                    data = list(map(float, row))
                    stre = None
                    if len(data) == 4:
                        stre = data.pop()
                    await post_client.send(sensor_type, *data, stre=stre)

        except KeyboardInterrupt:
            logger.info("KeyboardInterrupt detected. Exiting...")
        except Exception as e:
            logger.exception("Unexpected error: %s", e)
        finally:
            await post_client.close()

if __name__ == '__main__':
    asyncio.run(main())
