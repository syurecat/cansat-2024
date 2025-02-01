import math from 'mathjs'

class IMUCalculator {
    constructor() {
        this.x = math.zeros(3, 1); // 状態ベクトル
        this.P = math.eye(3); // 共分散行列
        this.Q = math.multiply(math.eye(3), 0.001); // プロセスノイズ
        this.R = math.multiply(math.eye(2), 0.01); // 観測ノイズ
        this.accel = [0, 0, 0]; // 加速度
    }

    // 状態遷移モデル
    f(x, u, dt) {
        let [wx, wy, wz] = u;
        let dx = math.multiply(dt, [[wx], [wy], [wz]]);
        let newX = math.add(x, dx);
        return newX;
    }

    // 状態遷移のヤコビアン
    calc_F() {
        return math.eye(3); // 角速度の積分なので単位行列
    }

    // 観測モデル（加速度計による傾き推定）
    h(x) {
        let theta_x = x.get([0, 0]);
        let theta_y = x.get([1, 0]);
        let hx = math.sin(theta_x);
        let hy = math.sin(theta_y);
        return math.matrix([[hx], [hy]]);
    }

    // 観測モデルのヤコビアン
    calc_H(x) {
        let theta_x = x.get([0, 0]);
        let theta_y = x.get([1, 0]);
        let Hx = math.cos(theta_x);
        let Hy = math.cos(theta_y);
        return math.matrix([
            [Hx, 0, 0],
            [0, Hy, 0]
        ]);
    }

    // EKF 予測ステップ
    predict(gyro, dt) {
        let F = this.calc_F();
        let predicted_x = this.f(this.x, gyro, dt);
        let P_temp = math.multiply(F, math.multiply(this.P, math.transpose(F)));
        let updated_P = math.add(P_temp, this.Q);

        this.x = predicted_x;
        this.P = updated_P;
    }

    // 加速度を保存
    storeAccel(accel) {
        this.accel = accel;
    }

    // EKF 更新ステップ
    update() {
        let H = this.calc_H(this.x);
        let y_res = math.subtract(math.matrix([[this.accel[0]], [this.accel[1]]]), this.h(this.x));
        let S = math.add(math.multiply(H, math.multiply(this.P, math.transpose(H))), this.R);
        let K = math.multiply(this.P, math.multiply(math.transpose(H), math.inv(S)));

        let updated_x = math.add(this.x, math.multiply(K, y_res));
        let I = math.eye(3);
        let updated_P = math.multiply(math.subtract(I, math.multiply(K, H)), this.P);

        this.x = updated_x;
        this.P = updated_P;
    }

    // 角度を取得
    getAngle() {
        return this.x.valueOf().map(row => row[0]);
    }
}

export default IMUCalculator;