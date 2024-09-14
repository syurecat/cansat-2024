from fastapi import FastAPI
import io
from fastapi.responses import StreamingResponse
from InfluxDB_data_acquisition import get_imu_data
from visualization_3d import generate_3d_plot
import matplotlib.pyplot as plt

app = FastAPI()

@app.get("/attitude_image")
async def get_attitude_image():
    # InfluxDBからデータを取得
    sensor_data = get_sensor_data()  # InfluxDBから取得
    pitch, roll, yaw = sensor_data['pitch'], sensor_data['roll'], sensor_data['yaw']

    # 3Dプロットを生成
    fig = generate_3d_plot(pitch, roll, yaw)

    # 画像をバッファに保存
    buf = io.BytesIO()
    plt.savefig(buf, format='png')
    buf.seek(0)
    
    return StreamingResponse(buf, media_type="image/png")