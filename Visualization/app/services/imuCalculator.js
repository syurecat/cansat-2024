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
    this.accel = acc;
  }

  updateMag(mag) {
    this.magnet = mag;
  }

  updateSensorData() {
    this.ahrs.update(this.gyro, this.accel, this.magnet);
  }

  getQuaternion() {
    return this.ahrs.getQuaternion();
  }
}

export default SensorAhrs;
