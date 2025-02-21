#include <SoftwareSerial.h>
#include "./SD.h"
#include "./IMU.h"
#include "./GPS.h"
#include "./LED.h"
#include "./GLOBALS.h"

SoftwareSerial MWSerial(2, 3); // RX, TX

bool errorFlag = false;
const int DIN_PIN = 7;
uint8_t gpsQuality = 0;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    MWSerial.begin(115200);
    LED_Init();
    pinMode( DIN_PIN, INPUT );
    pinMode( 6, INPUT_PULLUP );
    attachInterrupt(digitalPinToInterrupt(6),sep,FALLING);
    delay(300);

    SD_Init() ? MWSerial.println("SD_init_done.") : MWSerial.println("SD: init failed!");
    GPS_Init();
    BTH_Init();
    IMU_Init();
    Serial.println(F("Init done."));
    MWSerial.println(F("Init done."));
}

void loop() {
    gpsQuality = GPS_Update();
    LED_States(gpsQuality);
    IMU_UpdateAll();
    BTH_Update();
    // 加速度センサのX軸の値を取得
    float acc_x = IMU_GetAccX();
    // 加速度センサのY軸の値を取得
    float acc_y = IMU_GetAccY();
    // 加速度センサのZ軸の値を取得
    float acc_z = IMU_GetAccZ();
    // ジャイロセンサのX軸の値を取得s
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
    // 気圧の値を取得
    float prs = BTH_GetPressure();
    // 湿度の値を取得
    float hum = BTH_GetHumidity();
    // 温度の値を取得
    float tmp = BTH_GetTemperature();
    // 経度の値を取得
    float lat = GPS_GetLat();
    // 緯度の値を取得
    float lng = GPS_GetLng();
    // 高度の値を取得
    float height = GPS_GetHeight();


    int value = digitalRead( DIN_PIN );

    if ( value == LOW ){
        MWSerial.println("BME,"
            + String(tmp) + ","
            + String(hum) + ","
            + String(prs)
        );
        MWSerial.println("ACC,"
            + String(acc_x) + ","
            + String(acc_y) + ","
            + String(acc_z)
        );
        MWSerial.println("GYR,"
            + String(gyr_x) + ","
            + String(gyr_y) + ","
            + String(gyr_z)
        );
        MWSerial.println("MAG,"
            + String(mag_x) + ","
            + String(mag_y) + ","
            + String(mag_z)
        );
        MWSerial.println("GPS,"
            + String(lat) + ","
            + String(lng) + ","
            + String(height)
        );
    }

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
        + String(tmp)   + ","
        + String(hum)   + ","
        + String(prs)   + ","
        + String(lat)   + ","
        + String(lng)   + ","
        + String(height)+ ","
    );

    LED_Error(errorFlag);
    errorFlag = false;
}

void sep(){
    Serial.println(F("SEPA"));
}