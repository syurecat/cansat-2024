const char* labels[] = {"$BME", "$ACC", "$GYR", "$MAG", "$GPS"};  // 各センサーのラベル
int ranges[5][3] = {  // 各データ範囲 (min, max, scale factor)
    {2500, 3500, 100},  // BME
    {-500, 500, 100},   // ACC
    {-200, 200, 100},   // GYR
    {-2000, 2000, 1},   // MAG
    {0, 100, 100}       // GPS
};




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
        for (int j = 0; j < 3; j++) {
            Serial.print(random(ranges[i][0], ranges[i][1]) / (float)ranges[i][2]);
            Serial.print(",");
        }

        Serial.println(random(150, 200));  // 品質の出力
    }

    delay(200);  // 1秒間隔で出力
}