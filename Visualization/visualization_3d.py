import numpy as np
import matplotlib.pyplot as plt

# 円柱の頂点を生成する関数
def create_cylinder(radius, height, num_segments):
    theta = np.linspace(0, 2 * np.pi, num_segments)
    x = radius * np.cos(theta)
    y = radius * np.sin(theta)
    z = np.array([0, height])
    return x, y, z

# 回転行列を生成する関数
def rotation_matrix(pitch, roll, yaw):
    Rx = np.array([[1, 0, 0],
                [0, np.cos(pitch), -np.sin(pitch)],
                [0, np.sin(pitch), np.cos(pitch)]])
    
    Ry = np.array([[np.cos(roll), 0, np.sin(roll)],
                [0, 1, 0],
                [-np.sin(roll), 0, np.cos(roll)]])
    
    Rz = np.array([[np.cos(yaw), -np.sin(yaw), 0],
                [np.sin(yaw), np.cos(yaw), 0],
                [0, 0, 1]])
    
    return Rz @ Ry @ Rx

# ピッチ、ロール、ヨーの角度を受け取って円柱を描画する関数
def generate_3d_plot(pitch, roll, yaw, radius=1, height=5, num_segments=100):
    # 円柱の座標を生成
    x, y, z = create_cylinder(radius, height, num_segments)

    # 回転行列を計算
    R = rotation_matrix(pitch, roll, yaw)

    # 円柱を描画する準備
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # 回転後の座標を計算
    X, Y, Z = [], [], []
    for i in z:
        X_row, Y_row, Z_row = [], [], []
        for j in range(len(x)):
            v = np.array([x[j], y[j], i])
            rotated_v = R @ v
            X_row.append(rotated_v[0])
            Y_row.append(rotated_v[1])
            Z_row.append(rotated_v[2])
        X.append(X_row)
        Y.append(Y_row)
        Z.append(Z_row)
    
    X = np.array(X)
    Y = np.array(Y)
    Z = np.array(Z)

    # 円柱を描画
    ax.plot_surface(X, Y, Z, color='b', alpha=0.6)

    # 軸ラベル
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    plt.show()

# 使用例: ピッチ、ロール、ヨーの角度を設定して円柱を描画
pitch = np.radians(30)
roll = np.radians(20)
yaw = np.radians(45)

generate_3d_plot(pitch, roll, yaw)
