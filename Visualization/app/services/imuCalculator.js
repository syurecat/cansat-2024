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
    this.accel = acc.map(value => value / 9.81);
  }

  updateMag(mag) {
    this.magnet = mag;
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
