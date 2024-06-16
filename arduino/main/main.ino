#include <SoftwareSerial.h>
#include "./SD.h"
#include "./IMU.h"

SoftwareSerial MWSerial(2, 3); // RX, TX

const int DIN_PIN = 7;

void setup() {
    Wire.begin();
    Serial.begin(38400);
    MWSerial.begin(38400);
    pinMode( DIN_PIN, INPUT );
    delay(250);

    SD_init() ? MWSerial.println("SD_init_done.") : MWSerial.println("SD: init failed!");
    IMU_Init();
    Serial.println(F("Init done."));
    MWSerial.println(F("Init done."));
}

void loop() {
    IMU_UpdateAll();
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

    SD_Write(","
        + String(acc_x) + ","
        + String(acc_y) + ","
        + String(acc_z) + ","
        + String(gyr_x) + ","
        + String(gyr_y) + ","
        + String(gyr_z) + ","
        + String(mag_x) + ","
        + String(mag_y) + ","
        + String(mag_z) + ","
    );

    int value = digitalRead( DIN_PIN );

    if ( value == LOW ){
        MWSerial.println("acc,"
            + String(acc_x) + ","
            + String(acc_y) + ","
            + String(acc_z) + ","
        );
        MWSerial.println("gyr,"
            + String(gyr_x) + ","
            + String(gyr_y) + ","
            + String(gyr_z) + ","
        );
        MWSerial.println("mag,"
            + String(mag_x) + ","
            + String(mag_y) + ","
            + String(mag_z) + ","
        );
    }
}
