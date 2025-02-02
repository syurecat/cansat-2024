import { matrix, multiply, add, subtract, transpose, inv, identity } from 'mathjs';

// 初期状態 (オイラー角 [roll, pitch, yaw])
let x = matrix([[0], [0], [0]]);

// 状態誤差共分散行列
let P = identity(3);

// 状態遷移ノイズ
const Q = matrix([
    [0.001, 0, 0],
    [0, 0.001, 0],
    [0, 0, 0.001]
]);

// 観測ノイズ
const R = matrix([
    [0.01, 0],
    [0, 0.01]
]);

export function predict(gyr, dt) {
    const [gx, gy, gz] = gyr;
    const roll = x.get([0, 0]);
    const pitch = x.get([1, 0]);
    
    // 状態遷移モデル (ジャイロによるオイラー角の更新)
    const F = matrix([
        [1, Math.sin(roll) * Math.tan(pitch) * dt, Math.cos(roll) * Math.tan(pitch) * dt],
        [0, Math.cos(roll) * dt, -Math.sin(roll) * dt],
        [0, Math.sin(roll) / Math.cos(pitch) * dt, Math.cos(roll) / Math.cos(pitch) * dt]
    ]);
    
    const u = matrix([[gx * dt], [gy * dt], [gz * dt]]);
    x = add(x, multiply(F, u));
    P = add(multiply(F, multiply(P, transpose(F))), Q);
}

export function update(acc) {
    const [ax, ay, az] = acc;
    const roll_meas = Math.atan2(ay, az);
    const pitch_meas = Math.atan2(-ax, Math.sqrt(ay * ay + az * az));
    
    const H = matrix([
        [1, 0, 0],
        [0, 1, 0]
    ]);
    
    const z = matrix([[roll_meas], [pitch_meas]]);
    const y = subtract(z, multiply(H, x));
    
    const S = add(multiply(H, multiply(P, transpose(H))), R);
    const K = multiply(multiply(P, transpose(H)), inv(S));
    
    x = add(x, multiply(K, y));
    P = multiply(subtract(identity(3), multiply(K, H)), P);
}

export function getEulerAngles() {
    return x.toArray().flat();
}
