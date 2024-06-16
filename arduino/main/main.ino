#include <SoftwareSerial.h>
#include "./IMU.h"

SoftwareSerial MWSerial(2, 3); // RX, TX

const int DIN_PIN = 7;

void setup() {
    Wire.begin();
    Serial.begin(38400);
    MWSerial.begin(38400);
    pinMode( DIN_PIN, INPUT );

    p = buf;
    memset(buf, 0, sizeof(buf));
    IMU_Init();
    Serial.println(F("Init done"));
}

void loop() {
    // 加速度センサのX軸の値を取得
	float acc_x = IMU_GetAccX();
	// 加速度センサのY軸の値を取得
	float acc_y = IMU_GetAccY();
	// 加速度センサのZ軸の値を取得
	float acc_z = IMU_GetAccZ();
	// ジャイロセンサのX軸の値を取得
	float gyr_x = IMU_GetGyrX();
	// ジャイロセンサのY軸の値を取得
	float gyr_y = IMU_GetGyrY();
	// ジャイロセンサのZ軸の値を取得
	float gyr_z = IMU_GetGyrZ();
	// 磁気センサのX軸の値を取得
	int mag_x = IMU_GetMagX();
	// 磁気センサのY軸の値を取得
	int mag_y = IMU_GetMagY();
	// 磁気センサのZ軸の値を取得
	int mag_z = IMU_GetMagZ();

    int value = digitalRead( DIN_PIN );

    if ( value == LOW ){
        MWSerial.write("acc,"
            + String(acc_x) + ","
            + String(acc_y) + ","
            + String(acc_z) + "¥n"
        );
        MWSerial.write("gyr,"
            + String(gyr_x) + ","
            + String(gyr_y) + ","
            + String(gyr_z) + "¥n"
        );
        MWSerial.write("mag,"
            + String(mag_x) + ","
            + String(mag_y) + ","
            + String(mag_z) + "¥n"
        );
    }
}
