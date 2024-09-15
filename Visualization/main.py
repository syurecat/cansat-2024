from fastapi import FastAPI
import io
import math
from fastapi.responses import StreamingResponse
from InfluxDB_data_acquisition import get_imu_data
from visualization_3d import generate_3d_plot
import matplotlib.pyplot as plt


def calculate_pitch_roll(accel_data):
    ax = accel_data['x']
    ay = accel_data['y']
    az = accel_data['z']
    
    # ピッチとロールを計算
    pitch = math.atan2(ay, math.sqrt(ax**2 + az**2))
    roll = math.atan2(-ax, az)
    
    # ラジアンから度への変換
    pitch_deg = math.degrees(pitch)
    roll_deg = math.degrees(roll)
    
    return pitch_deg, roll_deg

#### ヨー角の計算

def calculate_yaw(mag_data, pitch, roll):
    mx = mag_data['x']
    my = mag_data['y']
    mz = mag_data['z']
    
    # ピッチとロールをラジアンに変換
    pitch_rad = math.radians(pitch)
    roll_rad = math.radians(roll)
    
    # 補正された磁力計の値を計算
    mx2 = mx * math.cos(pitch_rad) + mz * math.sin(pitch_rad)
    my2 = mx * math.sin(roll_rad) * math.sin(pitch_rad) + my * math.cos(roll_rad) - mz * math.sin(roll_rad) * math.cos(pitch_rad)
    
    # ヨー角を計算
    yaw = math.atan2(-my2, mx2)
    yaw_deg = math.degrees(yaw)
    
    return yaw_deg

app = FastAPI()

@app.get("/attitude_image")
async def get_attitude_image():
    # InfluxDBからデータを取得
    imu_data = get_imu_data()  # InfluxDBから取得
    pitch, roll, yaw = get_imu_data()

    # 3Dプロットを生成
    fig = generate_3d_plot(pitch, roll, yaw)

    # 画像をバッファに保存
    buf = io.BytesIO()
    plt.savefig(buf, format='png')
    buf.seek(0)
    
    return StreamingResponse(buf, media_type="image/png")
