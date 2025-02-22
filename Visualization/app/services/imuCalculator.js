import Ahrs from 'ahrs';

class SensorAhrs {
  constructor(options) {
    this.ahrs = new Ahrs(options);
    this.gyro = [0, 0, 0];
    this.accel = [0, 0, 0];
    this.magnet = [0, 0, 0];
  }

  updateGyro(gyr) {
    this.gyro = gyr;
  }

  updateAccel(acc) {
    this.accel = [acc[0] / 4.81, acc[1] / 4.81, -acc[2] / 4.81];
  }

  updateMag(mag) {
    // Z軸を反転
    this.magnet = [mag[0] / 100, mag[1] / 100, -mag[2] /100];
  }


  updateSensorData() {
    console.log(this.accel);
    console.log(this.gyro);
    console.log(this.magnet);
    this.ahrs.update(this.gyro[0], this.gyro[1], this.gyro[2], this.accel[0], this.accel[1], this.accel[2], this.magnet[0], this.magnet[1], this.magnet[2]);
  }

  getQuaternion() {
    return this.ahrs.getQuaternion();
  }
}

export default SensorAhrs;
