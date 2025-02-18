import aiohttp
import asyncio
import logging
from typing import Optional

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

class PostData:
    SENSOR_DATA_KEYS = {
        "ACC": ["x", "y", "z"],
        "GYR": ["x", "y", "z"],
        "MAG": ["x", "y", "z"],
        "BME": ["temp", "humi", "pres"],
        "GPS": ["lati", "long", "alti"],
    }

    def __init__(self, token: str, url: str, sensor_name: str) -> None:
        self.token = token
        self.url = url
        self.sensor_name = sensor_name
        self.session = None

    async def init(self):
        self.session = aiohttp.ClientSession()

    async def close(self) -> None:
        await self.session.close()

    async def send(self, sensor_type: str, *values: float, stre: Optional[float] = None) -> None:
        if sensor_type not in self.SENSOR_DATA_KEYS:
            logger.error("Unsupported sensor type: %s", sensor_type)
            return

        expected_keys = self.SENSOR_DATA_KEYS[sensor_type]
        if len(values) != len(expected_keys):
            logger.error(
                "Expected %d values for sensor type %s, but got %d",
                len(expected_keys), sensor_type, len(values)
            )
            return

        data_payload = dict(zip(expected_keys, values))

        # POST 用のペイロード作成
        payload = {
            "token": self.token,
            "type": sensor_type,
            "name": self.sensor_name,
            "data": data_payload
        }
        if stre is not None:
            payload["stre"] = stre

        try:
            async with self.session.post(self.url, json=payload) as response:
                if response.status == 200:
                    logger.info("Successfully posted %s data: %s", sensor_type, payload)
                else:
                    text = await response.json()
                    logger.error("Failed to post data (status: %s): %s", response.status, text)
        except Exception as e:
            logger.exception("Exception during POST: %s", e)
