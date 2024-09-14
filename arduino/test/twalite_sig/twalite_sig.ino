const char* labels[] = {"$BME", "$ACC", "$GYR", "$MAG", "$GPS"};  // 各センサーのラベル
int ranges[4][3] = {  // 各データ範囲 (min, max, scale factor)
    {2500, 3500, 100},  // BME
    {-500, 500, 100},   // ACC
    {-200, 200, 100},   // GYR
    {-2000, 2000, 1}    // MAG
};

float lat_range[2] = {24.0, 46.0};  // 緯度の範囲
float lon_range[2] = {123.0, 153.0};  // 経度の範囲


void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));
    delay(300);
    Serial.println(F("Init done."));
}
void loop() {
    for (int i = 0; i < 5; i++) {
        Serial.print(labels[i]);  // センサーラベル出力
        Serial.print(",");
        if (i < 4) {
            // BME, ACC, GYR, MAGの範囲で出力
            for (int j = 0; j < 3; j++) {
                Serial.print(random(ranges[i][0], ranges[i][1]) / (float)ranges[i][2]);
                Serial.print(",");
            }
        } else {
            // GPSの範囲で出力
            float lat = random(lat_range[0] * 100, lat_range[1] * 100) / 100.0;
            float lon = random(lon_range[0] * 100, lon_range[1] * 100) / 100.0;
            Serial.print(lat, 6);  // 緯度を6桁表示
            Serial.print(",");
            Serial.print(lon, 6);  // 経度を6桁表示
            Serial.print(",");
        }

        Serial.println(random(150, 200));  // 品質の出力
    }

    delay(400);  // 1秒間隔で出力
}
