from fastapi import FastAPI, WebSocket
from fastapi.responses import HTMLResponse
import uvicorn
from InfluxDB_data_acquisition import get_imu_data

app = FastAPI()

@app.get("/")
async def get():
    with open("index.html", "r") as file:
        return HTMLResponse(content=file.read())

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        # センサーからデータを受け取る
        data = get_imu_data()
        await websocket.send_json(data)

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=4000)
